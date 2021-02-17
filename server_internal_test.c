/*
 * Copyright 2013-2017 Fabian Groffen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This is a clumpsy program to test the distribution of a certain input
 * set of metrics.  See also:
 * https://github.com/graphite-project/carbon/issues/485
 *
 * compile using something like this:
 * clang -o distributiontest -I. issues/distributiontest.c consistent-hash.c \
 * server.c queue.c md5.c dispatcher.c router.c aggregator.c -pthread -lm */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <signal.h>
#include <libgen.h>
#include <unistd.h>

#include <event2/thread.h>
#include <event2/event.h>

#include "router.h"
#include "server_internal.h"

#include "listener_mock.h"

#define SRVCNT 8
#define REPLCNT 2

int relaylog(enum logdst dest, const char *fmt, ...) {
    (void) dest;
    (void) fmt;
    return 0;
}

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
extern char *pemcert;
#endif

unsigned char mode = 0;
static int batchsize = 2500;
static int queuesize = 250000;
static int maxstalls = 4;
static unsigned short iotimeout = 600;
static int sockbufsize = 0;

CTEST_DATA(sock_strm) {
    z_strm *strm;
};

CTEST_SETUP(sock_strm) {
    server_socketnew(&data->strm, METRIC_BUFSIZ);
}

CTEST_TEARDOWN(sock_strm) {
    if (data->strm) {
        data->strm->strmfree(data->strm);
    }

}

CTEST2(sock_strm, sock_write) {
    size_t i;
    size_t nmetrics = 4;
    char *metrics[4];
    metrics[0] = "AB.C 12 3\n";
    metrics[1] = "D.EF 356.0 12\n";
    metrics[2] = "D;a=B;c=E 586.2 27\n";
    metrics[3] = "K.L 98.0 464\n";
    char buf[METRIC_BUFSIZ];
    buf[0] = '\0';

    ASSERT_NOT_NULL_D(data->strm, "strm init");

    for (i = 0; i < nmetrics; i++) {
        char *b = server_strmbuf(data->strm);

        data->strm->strmwrite(data->strm, metrics[i], strlen(metrics[i]));
        strcat(buf, metrics[i]);
        b[server_strmbuflen(data->strm)] = '\0';
        ASSERT_STR_D(buf, b, "buffer mismatch");
    }
}

static char *metricll(long long n) {
    char *s = malloc(256);
    if (s != NULL) {
        snprintf(s, 256, "%lld.test %lld %lld\n", n, n, n);
    }
    return s;
}

void connect_and_send(server *s, listener_mock *d, int repeat_delay) {
    int i;
    z_strm *strm;
    const char *err;
    int state = DSTATUS_UP;
    int ret = server_connect(s, 0);
    struct pollfd ufd;
    // connection must failed, mock is down state
    ASSERT_EQUAL_D(-1, ret, "connection must failed");

    ret = listener_mock_set_state(d, state);
    ASSERT_EQUAL(0, ret);

    for (i = 0; i < 30; i++) {
        usleep(10); /* sleep for wake up event loop thread */
        if ((ret = server_connect(s, 0)) != -1) {
            break;
        }
    }

    server_poll(s, 0, &ufd);

    strm = server_get_strm(s, 0);

    // connection must successed
    if (ret == -1) {
        err = listener_get_err(d);
        LOG("%s: listener error", err);
        ASSERT_FORMAT("connection must successed: %s", strm->strmerror(strm, 0));
    }

    for (i = 0; i < queuesize; i++) {
        const char *m = metricll(i);
        size_t mlen = strlen(m);
        ssize_t slen = strm->strmwrite(strm, m, mlen);
        if (slen == -1 && errno == ENOBUFS) {
            /* Flush and retry */
            if (repeat_delay) {
                if (state == DSTATUS_UP) {
                    state = DSTATUS_DELAY;
                    ASSERT_EQUAL(0, listener_mock_set_state(d, state));
                }
                ret = strm->strmflush(strm);
                if (ret == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        state = DSTATUS_UP;
                        ASSERT_EQUAL(0, listener_mock_set_state(d, state));
                        ASSERT_EQUAL_D(1, server_poll(s, 0, &ufd), "poll");
                    } else {
                        ASSERT_FORMAT("%s\n", strerror(errno));
                    }
                }
            }

            if (strm->strmflush(strm) == 0) {
                slen = strm->strmwrite(strm, m, mlen);
            } else {
                ASSERT_FORMAT("strmflush error: %s", strerror(errno));
            }
        }
        free((void *) m);
        err = listener_get_err(d);
        ASSERT_STR_D(NULL, err, "listener error");

        ASSERT_EQUAL_D(mlen, slen, strerror(errno));
    }
    if (repeat_delay) {
        if (state == DSTATUS_DELAY) {
            state = DSTATUS_UP;
            ASSERT_EQUAL(0, listener_mock_set_state(d, state));
        }
    }
    ASSERT_EQUAL_D(0, strm->strmflush(strm), strerror(errno));

    sleep(1);
    for (i = 0; i < 20; i++) {
        /* delay loop for tests under valgring */
        usleep(100);
        if (queue_len(d->q) >= queuesize) {
            break;
        }
    }
    ASSERT_EQUAL_D(queuesize, queue_len(d->q), "queue elements too small");

    /* Verify received */
    for (i = 0; i < queuesize; i++) {
        const char *m = queue_dequeue(d->q);
        if (m == NULL) {
            ASSERT_STEP_D(i, "queue elements count too small");
        } else {
            char *m_cmp = metricll(i);
            ASSERT_STR_D(m_cmp, m, "mismatch queue item");
            free((void *) m);
            free((void *) m_cmp);
        }
    }

    server_disconnect(s, 0);
    listener_mock_stop(d);
    ASSERT_NULL_D(queue_dequeue(d->q), "queue not empthy");
    ASSERT_EQUAL_D(queuesize, i, "dequeue loop ended too early");
    ASSERT_EQUAL_D(queuesize, d->metrics, "queue elements received count too small");
}

CTEST_DATA(server_plain_tcp) {
    listener_mock d;
    char *ip;
    int port;
    con_proto proto;
    con_trnsp transport;
    struct addrinfo *saddr;
    struct addrinfo *hint; /* free in server_cleanup */
    server *s;
};

CTEST_SETUP(server_plain_tcp) {
    data->ip = "127.0.0.1";
    data->transport = W_PLAIN;
    data->port = listener_mock_init(&data->d, data->ip, 0, CON_TCP,
                                    data->transport, 1024, queuesize);
    data->saddr = NULL;
    data->proto = CON_TCP;
    data->hint = malloc(sizeof(struct addrinfo));
    hint_proto(data->hint, data->proto);
    data->s = NULL;
}

CTEST_TEARDOWN(server_plain_tcp) {
    if (data->s != NULL) {
        server_disconnect(data->s, 0);
        server_cleanup(data->s);
    }
    listener_mock_stop(&data->d);
    listener_mock_free(&data->d);
}

CTEST2(server_plain_tcp, connect_and_send) {
    if (data->port == -1) {
        LOG("dispatcher mock start: %s\n", listener_get_err(&data->d));
        ASSERT_NOT_EQUAL(-1, data->port);
    }

    data->s = server_new(data->ip, data->port, T_LINEMODE, data->transport,
                         data->proto, 1, data->saddr, data->hint, queuesize, NULL,
                         batchsize, maxstalls, iotimeout, sockbufsize, batchsize, batchsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d, 1);
}

#ifdef HAVE_GZIP
CTEST_DATA(server_gzip_tcp) {
    listener_mock d;
    char *ip;
    int port;
    con_proto proto;
    con_trnsp transport;
    struct addrinfo *saddr;
    struct addrinfo *hint; /* free in server_cleanup */
    server *s;
};

CTEST_SETUP(server_gzip_tcp) {
    data->ip = "127.0.0.1";
    data->transport = W_GZIP;
    data->port = listener_mock_init(&data->d, data->ip, 0, CON_TCP,
                                    data->transport, 1024, queuesize);
    data->saddr = NULL;
    data->proto = CON_TCP;
    data->hint = malloc(sizeof(struct addrinfo));
    hint_proto(data->hint, data->proto);
    data->s = NULL;
}

CTEST_TEARDOWN(server_gzip_tcp) {
    if (data->s != NULL) {
        server_disconnect(data->s, 0);
        server_cleanup(data->s);
    }
    listener_mock_stop(&data->d);
    listener_mock_free(&data->d);
}

CTEST2(server_gzip_tcp, connect_and_send) {
    if (data->port == -1) {
        LOG("dispatcher mock start: %s\n", listener_get_err(&data->d));
        ASSERT_NOT_EQUAL(-1, data->port);
    }

    data->s = server_new(data->ip, data->port, T_LINEMODE, data->transport,
                         data->proto, 1, data->saddr, data->hint, queuesize, NULL,
                         batchsize, maxstalls, iotimeout, sockbufsize, batchsize, batchsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d, 1);
}
#endif

#ifdef HAVE_LZ4_BAD_SOMETIMES_NO_BUFFER_SPACE_AVAIL
CTEST_DATA(server_lz4_tcp) {
    listener_mock d;
    char *ip;
    int port;
    con_proto proto;
    con_trnsp transport;
    struct addrinfo *saddr;
    struct addrinfo *hint; /* free in server_cleanup */
    server *s;
};

CTEST_SETUP(server_lz4_tcp) {
    data->ip = "127.0.0.1";
    data->transport = W_LZ4;
    data->port = listener_mock_init(&data->d, data->ip, 0, CON_TCP,
                                    data->transport, 1024, queuesize);
    data->saddr = NULL;
    data->proto = CON_TCP;
    data->hint = malloc(sizeof(struct addrinfo));
    hint_proto(data->hint, data->proto);
    data->s = NULL;
}

CTEST_TEARDOWN(server_lz4_tcp) {
    if (data->s != NULL) {
        server_disconnect(data->s);
        server_cleanup(data->s);
    }
    listener_mock_stop(&data->d);
    listener_mock_free(&data->d);
}

CTEST2(server_lz4_tcp, connect_and_send) {
    if (data->port == -1) {
        LOG("dispatcher mock start: %s\n", listener_get_err(&data->d));
        ASSERT_NOT_EQUAL(-1, data->port);
    }

    data->s = server_new(data->ip, data->port, T_LINEMODE, data->transport,
                         data->proto, data->saddr, data->hint, queuesize,
                         batchsize, maxstalls, iotimeout, sockbufsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d, 1);
}
#endif

#ifdef HAVE_SNAPPY
/* snappy implementation was buggy, not tested */
#endif

#ifdef HAVE_SSL_DISABLED
CTEST_DATA(server_ssl_tcp) {
    listener_mock d;
    char *ip;
    int port;
    con_proto proto;
    con_trnsp transport;
    struct addrinfo *saddr;
    struct addrinfo *hint; /* free in server_cleanup */
    server *s;
};

CTEST_SETUP(server_ssl_tcp) {
    data->ip = "127.0.0.1";
    data->transport = W_SSL | W_PLAIN;
    data->port = listener_mock_init(&data->d, data->ip, 0, CON_TCP,
                                    data->transport, 1024, queuesize);
    data->saddr = NULL;
    data->proto = CON_TCP;
    data->hint = malloc(sizeof(struct addrinfo));
    hint_proto(data->hint, data->proto);
    data->s = NULL;
}

CTEST_TEARDOWN(server_ssl_tcp) {
    if (data->s != NULL) {
        server_disconnect(data->s);
        server_cleanup(data->s);
    }
    listener_mock_stop(&data->d);
    listener_mock_free(&data->d);
}

CTEST2(server_ssl_tcp, connect_and_send) {
    if (data->port == -1) {
        LOG("dispatcher mock start: %s\n", listener_get_err(&data->d));
        ASSERT_NOT_EQUAL(-1, data->port);
    }

    data->s = server_new(data->ip, data->port, T_LINEMODE, data->transport,
                         data->proto, data->saddr, data->hint, queuesize,
                         batchsize, maxstalls, iotimeout, sockbufsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d, 1);
}
#endif

int main(int argc, const char *argv[]) {
    int ret;
#ifdef HAVE_SSL
    const char *dir = dirname((char *) argv[0]);
    pemcert = malloc(strlen(dir) + 25);    
    sprintf(pemcert, "%s/%s", dir, "test/buftest.ssl.cert");
#endif    
    signal(SIGPIPE, SIG_IGN);
    evthread_use_pthreads();
    ret = ctest_main(argc, argv);
#ifdef HAVE_LIBEVENT_SHUTDOWN
    /* for prevent sanitizers leak detect */
    libevent_global_shutdown();
#endif
#ifdef HAVE_SSL
    free(pemcert);
#endif    
    return ret;
}

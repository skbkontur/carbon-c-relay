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
#define CTEST_NOJMP

#include "ctest.h"

char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
#endif

unsigned char mode = 0;
static int batchsize = 2500;
static int queuesize = 250000;
static int maxstalls = 4;
static unsigned short iotimeout = 600;
static int sockbufsize = 0;

char server_connect(server *s);
int server_disconnect(server *s);
void server_cleanup(server *s);

z_strm *server_get_strm(server *s);

static char *ll2str(long long n) {
    char *s = malloc(256);
    if (s != NULL) {
        snprintf(s, 256, "%lld\n", n);
    }
    return s;
}

void connect_and_send(server *s, listener_mock *d) {
    int i;
    z_strm *strm;
    const char *err;
    int ret = server_connect(s);
    // connection must failed, mock is down state
    ASSERT_EQUAL_D(-1, ret, "connection must failed");

    ret = listener_mock_set_state(d, DSTATUS_UP);
    ASSERT_EQUAL(0, ret);

    for (i = 0; i < 30; i++) {
        usleep(10); /* sleep for wake up event loop thread */
        if ((ret = server_connect(s)) != -1) {
            break;
        }
    }
    // connection must successed
    ASSERT_NOT_EQUAL_D(-1, ret, "connection must successed");

    strm = server_get_strm(s);
    for (i = 0; i < queuesize; i++) {
        const char *m = ll2str(i);
        size_t mlen = strlen(m);
        ssize_t slen = strm->strmwrite(strm, m, mlen);
        if (slen == -1 && errno == ENOBUFS) {
            /* Flush and retry */
            if (strm->strmflush(strm) == 0) {
                slen = strm->strmwrite(strm, m, mlen);
            }
        }
        free((void *) m);
        err = listener_get_err(d);
        ASSERT_STR_D(NULL, err, "listener error");

        ASSERT_EQUAL_D(mlen, slen, strerror(errno));
    }
    ASSERT_EQUAL_D(0, strm->strmflush(strm), strerror(errno));

    usleep(200);

    /* Verify received */
    for (i = 0; i < queuesize; i++) {
        const char *m = queue_dequeue(d->q);
        if (m == NULL) {
            ASSERT_STEP_D(i, "queue elements count too small");
        } else {
            long long n = atoll(m);
            if (i != n) {
                ASSERT_EQUAL_D(i, n, "mismatch queue item");
            }
            free((void *) m);
        }
    }
    server_disconnect(s);
    listener_mock_stop(d);
    ASSERT_NULL_D(queue_dequeue(d->q), "queue not empthy");
    ASSERT_EQUAL_D(queuesize, d->metrics, "queue elements count too small");
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
        server_disconnect(data->s);
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
                         data->proto, data->saddr, data->hint, queuesize,
                         batchsize, maxstalls, iotimeout, sockbufsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d);
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
        server_disconnect(data->s);
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
                         data->proto, data->saddr, data->hint, queuesize,
                         batchsize, maxstalls, iotimeout, sockbufsize);
    ASSERT_NOT_NULL(data->s);

    connect_and_send(data->s, &data->d);
}
#endif

#ifdef HAVE_LZ4
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

    connect_and_send(data->s, &data->d);
}
#endif

#ifdef HAVE_SNAPPY
/* snappy implementation was buggy, not tested */
#endif

#ifdef HAVE_SSL
/* TODO: implement SSL tests */
#endif

int main(int argc, const char *argv[]) {
    int ret;
    signal(SIGPIPE, SIG_IGN);
    evthread_use_pthreads();
    ret = ctest_main(argc, argv);
#ifdef HAVE_LIBEVENT_SHUTDOWN
    /* for prevent sanitizers leak detect */
    libevent_global_shutdown();
#endif
    return ret;
}

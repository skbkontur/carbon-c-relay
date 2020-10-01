/*
 * Copyright 2013-2018 Fabian Groffen
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

#ifdef HAVE_GZIP
#include <zlib.h>
#endif
#ifdef HAVE_LZ4
#include <lz4.h>
#include <lz4frame.h>
#endif
#ifdef HAVE_SNAPPY
#include <snappy-c.h>
#endif
#ifdef HAVE_SSL
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>

#include "dispatcher_internal.h"
#include "router.h"

#include "listener_mock.h"

#define IDLE_DISCONNECT_TIME 600

static char srcaddr[24]; /* only for udp socket dispatcher internal
                            compability, not used in tests */

static char *_strndup(const char *s, size_t n) {
    char *d = malloc(n + 1);
    if (d) {
        strncpy(d, s, n);
        d[n] = '\0';
    }
    return d;
}

static void connclose(conn *c) {
    if (c->ev != NULL) {
        event_del(c->ev);
        event_free(c->ev);
        c->ev = NULL;
    }
    if (c->strm != NULL) {
        c->strm->strmclose(c->strm);
        c->strm = NULL;
    }
}

static size_t conn_dispatch(conn *c) {
    size_t metrics = 0;
    char *start = c->buf;
    char *p = start;
    char *end = start + c->buflen;
    while (p < end) {
        if (*p != '\n') {
            p++;
            continue;
        }
        p++;
        if (c->q != NULL) {
            char *m = _strndup(start, p - start);
            queue_enqueue(c->q, m);
        }
        metrics++;
        start = p;
    }
    if (start > c->buf) {
        size_t l = c->buflen - (start - c->buf);
        if (l > 0) {
            memmove(c->buf, start, l);
        }
        c->buflen = l;
    }
    return metrics;
}

static ssize_t connread(conn *c) {
    ssize_t metrics = 0;
    ssize_t len, bufsize;
    char state = __sync_fetch_and_add(c->state, 0);
    if (state == DSTATUS_DOWN || state == DSTATUS_RESET) {
        queue_enqueue(c->qerr, strdup("reset"));
        connclose(c);
        return 0;
    }
    bufsize = (sizeof(c->buf) - 1) - c->buflen;
    if (bufsize > 0) {
        char closed = 0;
        len = c->strm->strmread(c->strm, c->buf + c->buflen, bufsize);
        if (len > 0) {
            c->buflen += (size_t) len;
            metrics += conn_dispatch(c);
        } else if (len == 0) {
            closed = 1;
        } else if (len == -1 && (errno != EINTR && errno != EAGAIN &&
                                 errno != EWOULDBLOCK)) {
            queue_enqueue(c->qerr, strdup(strerror(errno)));
            closed = 1;
        }
        if (c->strm->strmreadbuf != NULL) {
            while (1) {
                bufsize = (sizeof(c->buf) - 1) - c->buflen;
                if ((len = c->strm->strmreadbuf(c->strm, c->buf + c->buflen,
                                                bufsize)) < 1) {
                    break;
                } else if (len > 0) {
                    c->buflen += (size_t) len;
                }
                metrics += conn_dispatch(c);
            }
        }
        if (closed) {
            connclose(c);
            return 0;
        } else {
            return metrics;
        }
    } else {
        errno = ENOBUFS;
        return -1;
    }
}

static void listener_read_cb(int fd, short flags, void *arg) {
    conn *c = (conn *) arg;

    if (flags & EV_TIMEOUT) {
        queue_enqueue(c->qerr, strdup("dispatch: read timeout on socket"));
        connclose(c);
    } else {
        ssize_t read = connread(c);
        if (read < 0 && errno == ENOBUFS) {
            queue_enqueue(c->qerr, strdup("too long message on socket"));
            connclose(c);
            return;
        } else if (read > 0) {
            c->d->metrics += read;
        }
    }
}

static void listener_accept_cb(int fd, short flags, void *arg) {
    listener_mock *d = (listener_mock *) arg;
    int client, state;
    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);

    if ((client = accept(fd, &addr, &addrlen)) < 0) {
        queue_enqueue(d->qerr, strdup(strerror(errno)));
        return;
    }
    state = __sync_fetch_and_add(&d->state, 0);

    switch (state) {
    case DSTATUS_UP:
    case DSTATUS_DELAY:
        (void) fcntl(client, F_SETFL, O_NONBLOCK);
        if (client >= d->connsize) {
            close(client);
            queue_enqueue(d->qerr, strdup("connections overflow"));
            return;
        }
        if (d->conns[client].strm == NULL) {
            d->conns[client].strm =
                connectionnew(client, srcaddr, d->ctype, d->transport, d->ctx);
            if (d->conns[client].strm == NULL) {
                queue_enqueue(d->qerr, strdup("error allocation connection"));
                close(client);
            } else {
                struct timeval tv;
                if (d->conns[client].ev != NULL) {
                    event_free(d->conns[client].ev);
                }
                d->conns[client].ev = event_new(
                    d->evbase, client, EV_TIMEOUT | EV_READ | EV_PERSIST,
                    listener_read_cb, &d->conns[client]);
                if (d->conns[client].ev == NULL) {
                    queue_enqueue(d->qerr,
                                  strdup("error allocation connection event"));
                    connclose(&d->conns[client]);
                }
                tv.tv_sec = IDLE_DISCONNECT_TIME;
                tv.tv_usec = 0;
                event_add(d->conns[client].ev, &tv);
                d->conns[client].buflen = 0;
            }
        } else {
            connclose(&d->conns[client]);
            queue_enqueue(d->qerr, strdup("duplicate connection"));
        }
        break;
    default:
        close(client);
    }
}

static void *listener_mock_runner(void *arg) {
    listener_mock *d = (listener_mock *) arg;

    if (d->ctype == CON_TCP) {
        if (listen(d->lsnr, 40) < 0) {
            __sync_bool_compare_and_swap(&d->running, 1, 0);
            queue_enqueue(d->qerr, strdup(strerror(errno)));
            goto EXIT;
        }
        d->ev = event_new(d->evbase, d->lsnr, EV_READ | EV_PERSIST,
                          listener_accept_cb, d);
    } else {
        d->ev = event_new(d->evbase, d->lsnr, EV_READ | EV_PERSIST,
                          listener_read_cb, d);
    }

    event_add(d->ev, NULL);

    if (d->ev == NULL) {
        __sync_bool_compare_and_swap(&d->running, 1, 0);
        queue_enqueue(d->qerr, strdup("unable create listener event"));
        goto EXIT;
    }

    __sync_bool_compare_and_swap(&d->running, 0, 1);

    event_base_loop(d->evbase, 0);

EXIT:
    __sync_bool_compare_and_swap(&d->running, 1, 0);
    return NULL;
}

int listener_mock_init(listener_mock *d, const char *ip, int port,
                       con_proto ctype, con_trnsp transport, size_t connsize,
                       size_t qsize) {
    size_t i;
    d->tid = 0;
    d->running = 0;
    d->lsnr = -1;
    d->metrics = 0;
    d->qerr = queue_new(24);
    d->ctype = ctype;
    d->transport = transport;

    d->state = DSTATUS_DOWN;
    switch (ctype) {
    case CON_TCP:
        d->lsnr = socket(AF_INET, SOCK_STREAM, 0);
        if (d->lsnr != -1) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            memset(&addr, 0, len);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip);
            memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
            if (bind(d->lsnr, (struct sockaddr *) &addr, len) == -1) {
                queue_enqueue(d->qerr, strdup(strerror(errno)));
                close(d->lsnr);
                d->lsnr = -1;
                return -1;
            }
            if (port == 0) {
                if (getsockname(d->lsnr, (struct sockaddr *) &addr, &len) ==
                    -1) {
                    queue_enqueue(d->qerr, strdup(strerror(errno)));
                    close(d->lsnr);
                    d->lsnr = -1;
                }
                d->port = ntohs(addr.sin_port);
            }
        }
        break;
    default:
        queue_enqueue(d->qerr, strdup("protocol not implemented"));
        return -1;
    }

    if (d->lsnr == -1) {
        queue_enqueue(d->qerr, strdup(strerror(errno)));
        return -1;
    }

    d->evbase = event_base_new();
    if (d->evbase == NULL) {
        close(d->lsnr);
        d->lsnr = -1;
        queue_enqueue(d->qerr, strdup("out of memory"));
        return -1;
    }

    if (qsize > 0) {
        d->q = queue_new(qsize);
        if (d->q == NULL) {
            close(d->lsnr);
            d->lsnr = -1;
            event_base_free(d->evbase);
            queue_enqueue(d->qerr, strdup("out of memory"));
            return -1;
        }
    } else {
        d->q = NULL;
    }
    d->connsize = connsize;
    d->conns = malloc(sizeof(conn) * connsize);
    if (d->conns == NULL) {
        // eventpipe_close(&d->notify_fd);
        close(d->lsnr);
        d->lsnr = -1;
        event_base_free(d->evbase);
        queue_destroy(d->q);
        queue_enqueue(d->qerr, strdup("out of memory"));
        return -1;
    }
    for (i = 0; i < connsize; i++) {
        d->conns[i].ev = NULL;
        d->conns[i].strm = NULL;
        d->conns[i].state = &d->state;
        d->conns[i].q = d->q;
        d->conns[i].qerr = d->qerr;
        d->conns[i].d = d;
    }

    return d->port;
}

int listener_mock_start(listener_mock *d) {
    if (d->tid == 0) {
        if (pthread_create(&d->tid, NULL, &listener_mock_runner, d) != 0) {
            return -1;
        }
    }
    return 0;
}

int listener_mock_stop(listener_mock *d) {
    if (d->tid > 0) {
        size_t i;
        __sync_lock_test_and_set(&d->state, DSTATUS_DOWN);
        if (event_base_loopexit(d->evbase, NULL) == -1) {
            abort();
        }
        pthread_join(d->tid, NULL);
        d->tid = 0;
        for (i = 0; i < d->connsize; i++) {
            connclose(&d->conns[i]);
        }
    }
    return 0;
}

int listener_mock_set_state(listener_mock *d, int state) {
    switch (state) {
    case DSTATUS_DOWN:
        listener_mock_stop(d);
        break;
    default: {
        int curr_state = __sync_fetch_and_add(&d->state, 0);
        if (curr_state == state) {
            return 0;
        }
        __sync_bool_compare_and_swap(&d->state, curr_state, state);
        if (d->tid == 0) {
            return listener_mock_start(d);
        }
        break;
    }
    }

    return 0;
}

void listener_mock_free(listener_mock *d) {
    if (d->lsnr != -1) {
        if (d->ev != NULL) {
            event_free(d->ev);
        }
        close(d->lsnr);
    }
    if (d->conns != NULL) {
        size_t i;
        for (i = 0; i < d->connsize; i++) {
            connclose(&d->conns[i]);
            connclose(&d->conns[i]);
        }
        free(d->conns);
        d->conns = NULL;
        d->connsize = 0;
    }

    if (d->evbase != NULL) {
        event_base_free(d->evbase);
    }

    if (d->qerr != NULL) {
        queue_destroy(d->qerr);
    }
}

const char *listener_get_err(listener_mock *d) {
    if (d->qerr == NULL) {
        return NULL;
    }
    return queue_dequeue(d->qerr);
}

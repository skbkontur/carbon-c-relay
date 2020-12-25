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

/* Init libevent with pthreads locks in main with
 * evthread_use_pthreads();
 */

#ifndef LISTENER_MOCK_H
#define LISTENER_MOCK_H 1

#include <pthread.h>
#include <string.h>

#ifdef HAVE_SSL
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

#include "queue.h"
#include "relay.h"

#define DSTATUS_DOWN 0
#define DSTATUS_UP 1
#define DSTATUS_DELAY 2
#define DSTATUS_RESET 3

typedef struct _z_strm z_strm;
typedef struct _listener_mock listener_mock;
typedef struct _conn {
    struct event *ev;
    z_strm *strm;
    char *buf;
    size_t bufsize;
    size_t buflen;
    int *state;
    queue *q;
    queue *qerr;
    listener_mock *d;
} conn;

struct _listener_mock {
    pthread_t tid;
    struct event_base *evbase;
    struct event *ev;
    int lsnr;

    queue *q;
    conn *conns;
    size_t connsize;
    size_t metrics;
    char running;
    int state;
    queue *qerr;
    const char *ip;
    int port;
    con_proto ctype;
    con_trnsp transport;
#ifdef HAVE_SSL
    SSL_CTX *ctx;
#else
    void *ctx;
#endif
};

int listener_mock_init(listener_mock *d, const char *ip, int port,
                       con_proto ctype, con_trnsp transport, size_t connsize,
                       size_t qsize);
int listener_mock_set_state(listener_mock *d, int state);
int listener_mock_stop(listener_mock *d);
void listener_mock_free(listener_mock *d);
const char *listener_get_err(listener_mock *d);

#endif /* LISTENER_MOCK_H */

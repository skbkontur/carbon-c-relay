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

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>

#include "dispatcher_internal.h"
#include "dispatcher.h"
#include "cluster.h"

#define SRVCNT 8
#define REPLCNT 2

unsigned char mode = 0;

char testdir[256];

int relaylog(enum logdst dest, const char *fmt, ...) {
    (void) dest;
    (void) fmt;
    return 0;
}

queue *server_queue(server *s);

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
#endif

CTEST_DATA(dispatcher_test) {
    router *r;
    dispatcher *d;
    connection *con;
};

CTEST_SETUP(dispatcher_test) {
    char path[256+16];
    sprintf(path, "%s/%s", testdir, "test.conf");

    data->r = router_readconfig(NULL, path, 2, 4096, 128, 1, 0, 0, 2003);
    ASSERT_NOT_NULL_D(data->r, path);
    data->d = dispatch_new(0, CONNECTION);
    dispatch_init(data->d, data->r, ";=", METRIC_BUFSIZ, METRIC_BUFSIZ);
    data->con = dispatch_addconnection(1, NULL, data->d, 0, 1);
}

CTEST_TEARDOWN(dispatcher_test) {
    dispatch_closeconnection(data->d, data->con, 0);
    dispatch_free(data->d);
}

CTEST2(dispatcher_test, dispatch_received_metrics) {
    size_t i;
    const char *metric, *m;    
    char *buf = " AB.C 12 3\n" "D.EF 356.0 12\n" "D;a=B;c=E 586.2 27\n" "K";
    size_t nmetrics = 4;
    char *metrics[4];
    metrics[0] = "AB.C 12 3\n";
    metrics[1] = "D.EF 356.0 12\n";
    metrics[2] = "D;a=B;c=E 586.2 27\n";
    metrics[3] = "K.L 98.0 464\n";

    connection_buf_cat(data->con, buf);
    dispatch_received_metrics(data->con, data->d);
    ASSERT_STR_D("K", connection_buf(data->con), "buffer after");
    
    connection_buf_cat(data->con, ".L 98.0 464\n");
    dispatch_received_metrics(data->con, data->d);
    ASSERT_STR_D("", connection_buf(data->con), "buffer after");

    queue *q = server_queue(*router_getservers(data->r));
    
    for (i = 0; i < nmetrics; i++) {
        metric = queue_dequeue(q);
        ASSERT_NOT_NULL_D(metric, metrics[i]);
        m = metric + sizeof(metric);
        ASSERT_STR(metrics[i], m);
    }

    metric = queue_dequeue(q);
    ASSERT_NULL_D(metric, "queue not empthy");
}

int main(int argc, const char *argv[]) {
    char *dir = dirname((char *) argv[0]);
    snprintf(testdir, sizeof(testdir), "%s/test", dir);

    strcpy(relay_hostname, "relay");

    return ctest_main(argc, argv);
}

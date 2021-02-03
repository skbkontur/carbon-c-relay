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

#include <event2/thread.h>
#include <event2/event.h>

#include "queue.h"
#include "consistent-hash.h"
#include "router.h"
#include "server.h"
#include "server_internal.h"
#include "relay.h"
#include "md5.h"
#include "dispatcher.h"
#include "conffile.h"

#include "listener_mock.h"

#define SRVCNT 8
#define REPLCNT 2
#define DESTS_SIZE 32

int relaylog(enum logdst dest, const char *fmt, ...) {
    (void) dest;
    (void) fmt;
    return 0;
}

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

char testdir[256];

const char *m = "bla.bla.bla";

char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
#endif

static int queuesize = 100;
static int batchsize = 10;
unsigned char mode = 0;
//static int maxstalls = 4;
//static unsigned short iotimeout = 600;
//static int sockbufsize = 0;

CTEST_DATA(server_any_of2_plain_tcp) {
   	char config[280];
	router *rtr;
	cluster *cl;
    listener_mock d[2];
	int ports[2];
	char *ip;
    int port;
    con_proto proto;
    con_trnsp transport;
    struct addrinfo *saddr;
    struct addrinfo *hint; /* free in server_cleanup */
};

CTEST_SETUP(server_any_of2_plain_tcp) {
	size_t i;
    data->rtr = NULL;
    data->cl = NULL;
    snprintf(data->config, sizeof(data->config), "%s/any_of2.conf", testdir);

	data->ip = "127.0.0.1";
    data->transport = W_PLAIN;
	for (i = 0; i < 2; i++) {
    	data->ports[i] = listener_mock_init(&data->d[i], data->ip, 0, CON_TCP,
        	                            data->transport, 1024, queuesize);
	}
}

CTEST_TEARDOWN(server_any_of2_plain_tcp) {
	size_t i;
    if (data->rtr) {
	    router_free(data->rtr);
    }
	for (i = 0; i < 2; i++) {
		listener_mock_stop(&data->d[i]);
    	listener_mock_free(&data->d[i]);
	}
}

CTEST2(server_any_of2_plain_tcp, server_send) {
	size_t destlen = 0, len = 0, blackholed = 0;
	size_t send_metrics = 2 * queuesize - 4 * batchsize;
	size_t i, j;
	destination dests[DESTS_SIZE];
	char *metric;
	char *firstspace;
	size_t metrics = 0;
	size_t dropped = 0;

	queuefree_threshold_start = 1;
	queuefree_threshold_end = 3;
	

	data->rtr = router_readconfig(NULL, data->config, 1, queuesize,
					batchsize, 1, 600, 0, 2003);
	ASSERT_NOT_NULL_D(data->rtr, "router_readconfig failed");
	data->cl = router_cluster(data->rtr, "test");
	ASSERT_NOT_NULL_D(data->cl, "cluster test not found");

	for (i = 0; i < send_metrics; i++) {
		metric = strdup(m);
		firstspace = metric + strlen(metric);
		if (router_route(data->rtr, dests, &len, DESTS_SIZE, "127.0.0.1", metric, firstspace, 1) == 0) {
			for (j = 0; j < len; j++) {
				server_send(dests[j].dest, dests[j].metric, 0);
			}
			destlen += len;
		} else {
			blackholed++;
		}
	}
	ASSERT_EQUAL_U_D(blackholed, 0, "router_route blackholed");

	for (i = 0; i < data->cl->members.anyof->count; i++) {
		/* metrics += server_get_metrics(data->cl->members.anyof->servers[i]) + queue_len(server_queue(data->cl->members.anyof->servers[i])); */
        metrics += queue_len(server_queue(data->cl->members.anyof->servers[i]));
		dropped += server_get_dropped(data->cl->members.anyof->servers[i]);
	}
	ASSERT_EQUAL_U_D(send_metrics, metrics, "server_send send metrics mismatch");
	ASSERT_EQUAL_U_D(dropped, 0, "server_send drop");
}


int main(int argc, const char *argv[]) {
    int ret;
    char *dir = dirname((char *) argv[0]);
	snprintf(testdir, sizeof(testdir), "%s/test", dir);
    strcpy(relay_hostname, "relay");

    signal(SIGPIPE, SIG_IGN);
    evthread_use_pthreads();
    
    ret = ctest_main(argc, argv);
#ifdef HAVE_LIBEVENT_SHUTDOWN
    /* for prevent sanitizers leak detect */
    libevent_global_shutdown();
#endif
    return ret;
}

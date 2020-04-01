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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <libgen.h>

#include "queue.h"
#include "consistent-hash.h"
#include "router.h"
#include "server.h"
#include "relay.h"
#include "md5.h"
#include "conffile.h"

int
relaylog(enum logdst dest, const char *fmt, ...)
{
	(void) dest;
	(void) fmt;
	return 0;
}

router *router_new(void);
char *router_validate_address(router *rtr, char **retip, unsigned short *retport, void **retsaddr, void **rethint, char *ip, con_proto proto);
queue *server_queue(server *s);

#include "minunit.h"

#define DESTS_SIZE 32

char testdir[256];

const char *m = "bla.bla.bla";

unsigned char mode = 0;
char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
#endif

MU_TEST(server_send_failover) {
	size_t destlen = 0, len = 0, blackholed = 0;
	char config[280];
	int queuesize = 100;
	int batchsize = 10;
	size_t send_metrics = 2 * queuesize - 4 * batchsize;
	int i, j;
	destination dests[DESTS_SIZE];
	char *metric;
	char *firstspace;
	router *rtr;
	cluster *cl;

	strcpy(relay_hostname, "relay");

	snprintf(config, sizeof(config), "%s/%s", testdir, "failover2.conf");
	rtr = router_readconfig(NULL, config, 1, queuesize,
					batchsize, 1, 600, 0, 2003);
	if (rtr == NULL) {
		mu_fail("router_readconfig failed");
		return;
	}

	for (i = 0; i < send_metrics; i++) {
		metric = strdup(m);
		firstspace = metric + strlen(metric);
		if (router_route(rtr, dests, &len, DESTS_SIZE, "127.0.0.1", metric, firstspace, 1) == 0) {
			for (j = 0; j < len; j++) {
				server_send(dests[j].dest, dests[j].metric, 0);
			}
			destlen += len;
		} else {
			blackholed++;
		}
	}
	mu_assert(blackholed == 0, "router_route blackholed");
	cl = router_cluster(rtr, "test");
	if (cl == NULL) {
		mu_fail("cluster test not found");
	} else {
		size_t metrics = 0;
		size_t dropped = 0;

		for (i = 0; i < cl->members.anyof->count; i++) {
			metrics += queue_len(server_queue(cl->members.anyof->servers[i]));
			dropped += server_get_dropped(cl->members.anyof->servers[i]);
		}
		mu_assert(metrics == send_metrics, "server_send metrics count");
		mu_assert(dropped == 0, "server_send drop");
	}
	router_free(rtr);
}

MU_TEST(server_send_any_of) {
	size_t destlen = 0, len = 0, blackholed = 0;
	char config[280];
	int queuesize = 100;
	int batchsize = 10;
	size_t send_metrics = 2 * queuesize - 4 * batchsize;
	int i, j;
	destination dests[DESTS_SIZE];
	char *metric;
	char *firstspace;
	router *rtr;
	cluster *cl;

	strcpy(relay_hostname, "relay");

	snprintf(config, sizeof(config), "%s/%s", testdir, "any_of2.conf");
	rtr = router_readconfig(NULL, config, 1, queuesize,
					batchsize, 1, 600, 0, 2003);
	if (rtr == NULL) {
		mu_fail("router_readconfig failed");
		return;
	}

	for (i = 0; i < send_metrics; i++) {
		metric = strdup(m);
		firstspace = metric + strlen(metric);
		if (router_route(rtr, dests, &len, DESTS_SIZE, "127.0.0.1", metric, firstspace, 1) == 0) {
			for (j = 0; j < len; j++) {
				server_send(dests[j].dest, dests[j].metric, 0);
			}
			destlen += len;
		} else {
			blackholed++;
		}
	}
	mu_assert(blackholed == 0, "router_route blackholed");
	cl = router_cluster(rtr, "test");
	if (cl == NULL) {
		mu_fail("cluster test not found");
	} else {
		size_t metrics = 0;
		size_t dropped = 0;

		for (i = 0; i < cl->members.anyof->count; i++) {
			metrics += queue_len(server_queue(cl->members.anyof->servers[i]));
			dropped += server_get_dropped(cl->members.anyof->servers[i]);
		}
		mu_assert(metrics == send_metrics, "server_send metrics count");
		mu_assert(dropped == 0, "server_send drop");
	}
	router_free(rtr);
}

MU_TEST_SUITE(server_send_suite) {
	MU_RUN_TEST(server_send_failover);
	MU_RUN_TEST(server_send_any_of);
}

int main(int argc, char *argv[]) {
	char *dir = dirname(argv[0]);
	snprintf(testdir, sizeof(testdir), "%s/test", dir);
	MU_RUN_SUITE(server_send_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

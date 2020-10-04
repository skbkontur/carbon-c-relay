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

#include "consistent-hash.h"
#include "md5.h"
#include "relay.h"
#include "router.h"
#include "server.h"

#define SRVCNT 8
#define REPLCNT 2

unsigned char mode = 0;

int relaylog(enum logdst dest, const char *fmt, ...) {
    (void) dest;
    (void) fmt;
    return 0;
}

router *router_new(void);
char *router_validate_address(router *rtr, char **retip,
                              unsigned short *retport,
                              struct addrinfo **retsaddr,
                              struct addrinfo **rethint, char *ip,
                              con_proto proto);

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

char relay_hostname[256];
#ifdef HAVE_SSL
char *sslCA = NULL;
char sslCAisdir = 0;
#endif

CTEST_DATA(router_test) {
    router *r;
    struct addrinfo *retsaddr;
    struct addrinfo *rethint;
};

CTEST_SETUP(router_test) {
    data->r = router_new();
    data->retsaddr = NULL;
    data->rethint = NULL;
}

CTEST_TEARDOWN(router_test) {
    router_free(data->r);
    free(data->rethint);
    freeaddrinfo(data->retsaddr);
}

CTEST2(router_test, router_validate_address_hostname) {
    char *retip;
    unsigned short retport;
    char ip[256];

    strcpy(ip, "host");
    ASSERT_NULL(router_validate_address(data->r, &retip, &retport,
                                        &data->retsaddr, &data->rethint, ip,
                                        CON_TCP));
    ASSERT_EQUAL(retport, 2003);
    ASSERT_STR(retip, "host");
    free(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;

    strcpy(ip, "[host:1]");
    ASSERT_NULL(router_validate_address(data->r, &retip, &retport,
                                        &data->retsaddr, &data->rethint, ip,
                                        CON_TCP));
    ASSERT_EQUAL(retport, 2003);
    ASSERT_STR(retip, "host:1");
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;
}

CTEST2(router_test, router_validate_address_port) {
    char *retip;
    unsigned short retport;
    char ip[256];

    strcpy(ip, ":2005");
    ASSERT_NULL(router_validate_address(data->r, &retip, &retport,
                                        &data->retsaddr, &data->rethint, ip,
                                        CON_TCP));
    ASSERT_EQUAL(retport, 2005);
    ASSERT_NULL(retip);
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;

    strcpy(ip, ":2005a");
    ASSERT_STR(router_validate_address(data->r, &retip, &retport,
                                       &data->retsaddr, &data->rethint, ip,
                                       CON_TCP),
               "invalid port number '2005a'");
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;
}

CTEST2(router_test, router_validate_address_hostname_port) {
    char *retip;
    unsigned short retport;
    char ip[256];

    strcpy(ip, "host:2004");
    ASSERT_NULL(router_validate_address(data->r, &retip, &retport,
                                        &data->retsaddr, &data->rethint, ip,
                                        CON_TCP));
    ASSERT_EQUAL(retport, 2004);
    ASSERT_STR(retip, "host");
    free(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;

    strcpy(ip, "host:2005a");
    ASSERT_STR(router_validate_address(data->r, &retip, &retport,
                                       &data->retsaddr, &data->rethint, ip,
                                       CON_TCP),
               "invalid port number '2005a'");
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;

    strcpy(ip, "[host:1]:2002");
    ASSERT_NULL(router_validate_address(data->r, &retip, &retport,
                                        &data->retsaddr, &data->rethint, ip,
                                        CON_TCP));
    ASSERT_EQUAL(retport, 2002);
    ASSERT_STR(retip, "host:1");
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;

    strcpy(ip, "[host:1]:2006c");
    ASSERT_STR(router_validate_address(data->r, &retip, &retport,
                                       &data->retsaddr, &data->rethint, ip,
                                       CON_TCP),
               "invalid port number '2006c'");
    freeaddrinfo(data->rethint);
    data->rethint = NULL;
    freeaddrinfo(data->retsaddr);
    data->retsaddr = NULL;
}

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }

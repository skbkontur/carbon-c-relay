/*
 * Copyright 2013-2019 Fabian Groffen
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

#ifndef _CONFFILE_H
#define _CONFFILE_H 1

#define GRAPHITE_PORT 2003

#include "server.h"
#include "aggregator.h"
#include "allocator.h"
#include "receptor.h"
#include "cluster.h"

typedef struct _validate {
	struct _route *rule;
	enum { VAL_LOG, VAL_DROP } action;
} validate;

typedef struct _destinations {
	cluster *cl;
	struct _destinations *next;
} destinations;

typedef struct _route {
	char *pattern;    /* original regex input, used for printing only */
	regex_t *rule;    /* regex per worker on metric, only if type == REGEX */
	size_t nmatch;    /* number of match groups */
	char *strmatch;   /* string to search for if type not REGEX or MATCHALL */
	destinations *dests; /* where matches should go */
	char *masq;       /* when set, what to feed to the hashfunc when routing */
	char stop:1;      /* whether to continue matching rules after this one */
	enum {
		MATCHALL,     /* the '*', don't do anything, just match everything */
		REGEX,        /* a regex match */
		CONTAINS,     /* find string occurrence */
		STARTS_WITH,  /* metric must start with string */
		ENDS_WITH,    /* metric must end with string */
		MATCHES       /* metric matches string exactly */
	} matchtype;      /* how to interpret the pattern */
	struct _route *next;
} route;

void router_yyerror(void *locp, void *, router *r, allocator *ra, allocator *pa, const char *msg);
char *router_validate_address(router *rtr, char **retip, unsigned short *retport, void **retsaddr, void **rethint, char *ip, con_proto proto);
char *router_validate_path(router *rtr, char *path);
char *router_validate_expression(router *rtr, route **retr, char *pat);
char *router_validate_cluster(router *rtr, cluster **retcl, char *cluster);
char *router_add_server(router *ret, char *ip, unsigned short port, char *inst, con_type type, con_trnsp transport, con_proto proto, struct addrinfo *saddrs, struct addrinfo *hint, char useall, cluster *cl);
char *router_add_cluster(router *r, cluster *cl);
cluster *router_cluster(router *rtr, const char *clname);
char *router_add_route(router *r, route *rte);
char *router_add_aggregator(router *rtr, aggregator *a);
char *router_add_stubroute(router *rtr, enum clusttype type, cluster *w, destinations *dw);
char *router_add_listener(router *rtr, con_type ltype, con_trnsp trnsp, char *pemcert, con_proto ctype, char *ip, unsigned short port, struct addrinfo *saddrs);
char *router_set_statistics(router *rtr, destinations *dsts);
char *router_set_collectorvals(router *rtr, int val, char *prefix, col_mode m);

#endif

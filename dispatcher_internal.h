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


#ifndef DISPATCHER_INTERNAL_H
#define DISPATCHER_INTERNAL_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#include "dispatcher.h"

enum conntype {
	LISTENER,
	CONNECTION
};

typedef struct _z_strm {
	ssize_t (*strmread)(struct _z_strm *, void *, size_t);  /* read func */

	/* read from buffer only func, on error set errno to ENOMEM, EMSGSIZE or EBADMSG */
	ssize_t (*strmreadbuf)(struct _z_strm *, void *, size_t);
	int (*strmclose)(struct _z_strm *);
	const char *(*strmerror)(struct _z_strm *, int);     /* get last err str */
	union {
#ifdef HAVE_GZIP
		struct gz {
			z_stream z;
			int inflatemode;
		} gz;
#endif
#ifdef HAVE_LZ4
		struct lz4 {
			LZ4F_decompressionContext_t lz;
			size_t iloc; /* location for unprocessed input */
		} lz4;
#endif
#ifdef HAVE_SSL
		SSL *ssl;
#endif
		int sock;
		/* udp variant (in order to receive info about sender) */
		struct udp_strm {
			int sock;
			struct sockaddr_in6 saddr;
			char *srcaddr;
			size_t srcaddrlen;
		} udp;
	} hdl;
#if defined(HAVE_GZIP) || defined(HAVE_LZ4) || defined(HAVE_SNAPPY)
	char *ibuf;
	size_t ipos;
	size_t isize;
#endif
	struct _z_strm *nextstrm;
	char closing; /* for detect end read from compressed buffered streams */
} z_strm;

z_strm *
connection_strm_new(int sock, char *srcaddr, con_proto ctype, con_trnsp transport
			#ifdef HAVE_SSL
			, SSL_CTX *ctx
			#else
			, void *empthy
			#endif
			, char **obuf, size_t *osize
);

connection *connection_new();
dispatcher *dispatch_new(unsigned char id, enum conntype type);
void dispatch_init(dispatcher *d, router *r, char *allowed_chars, int maxinplen, int maxmetriclen);
void dispatch_closeconnection(dispatcher *d, connection *conn, ssize_t len);
void dispatch_received_metrics(connection *conn, dispatcher *self);

void connection_buf_cat(connection *con, char *data);
const char *connection_buf(connection *con);
const char *connection_metric(connection *con);

#endif

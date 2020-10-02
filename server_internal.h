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


#ifndef SERVER__INTERNAL_H
#define SERVER_INTERNAL_H 1

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

#include "server.h"

typedef struct _z_strm {
	ssize_t (*strmwrite)(struct _z_strm *, const void *, size_t);
	int (*strmflush)(struct _z_strm *);
	int (*strmclose)(struct _z_strm *);
	const char *(*strmerror)(struct _z_strm *, int);     /* get last err str */
	struct _z_strm *nextstrm;                            /* set when chained */
	char obuf[METRIC_BUFSIZ];
	int obuflen;
#ifdef HAVE_SSL
	SSL_CTX *ctx;
#endif
	union {
#ifdef HAVE_GZIP
		z_streamp gz;
#endif
#ifdef HAVE_LZ4
		struct {
			void *cbuf;
			size_t cbuflen;
		} z;
#endif
#ifdef HAVE_SSL
		SSL *ssl;
#endif
		int sock;
	} hdl;
} z_strm;

#endif
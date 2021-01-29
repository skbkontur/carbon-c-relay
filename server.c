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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <assert.h>

#include "relay.h"
#include "dispatcher.h"
#include "collector.h"
#include "server_internal.h"
#include "consistent-hash.h"
#include "cluster.h"

#define FAIL_WAIT_COUNT   6  /* 6 * 250ms = 1.5s */
#define DISCONNECT_WAIT_TIME   12  /* 12 * 250ms = 3s */
#define QUEUE_FREE_CRITICAL(FREE, s)  (FREE < s->qfree_threshold)

int queuefree_threshold_start = 0;
int queuefree_threshold_end = 0;
int shutdown_timeout = 120; /* 120s */

struct _server {
	const char *ip;
	unsigned short port;
	char *instance;
	struct addrinfo *saddr;
	struct addrinfo *hint;
	int fd;
	char reresolve:1;
	/* 
	 * queue shared with other servers (for FAILOVER or LOAD BALANCING)
	 * free queue in cluster/router cleanup
	 */
	char shared_queue:2;
	z_strm *strm;
	queue *queue;
	size_t bsize;
	size_t qfree_threshold;
	short iotimeout;
	unsigned int sockbufsize;
	unsigned char maxstalls:SERVER_STALL_BITS;
	const char **batch; /* dequeued metrics */
	// size_t nlen; /* size of dequeued messages, but not sended (no buffer space), usually 0 or 1 */
	size_t len; /* size of dequeued messages and written to buffer */
	con_type type;
	con_trnsp transport;
	con_proto ctype;
	pthread_t tid;
	struct _server **secondaries;
	size_t *secpos;
	size_t secondariescnt;
	char failover:1;
	struct timeval last;
	char failure; 	    /* full byte for atomic access */
	char alive;         /* full byte for atomic access */
	char running;       /* full byte for atomic access */
	char keep_running;  /* full byte for atomic access */
	unsigned char stallseq;  /* full byte for atomic access */
	size_t metrics;
	size_t dropped;
	size_t stalls;
	size_t ticks;
	size_t waits;
	size_t requeue;
	size_t prevmetrics;
	size_t prevdropped;
	size_t prevstalls;
	size_t prevticks;
	size_t prevwaits;
	size_t prevrequeue;
};


/* connection specific writers and closers */

/* ordinary socket */
static inline int sockflush(z_strm *strm);

static inline ssize_t
sockwrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ) {
		/* need to flush */
		errno = ENOBUFS;
		return -1;
	}

	/* append metric to buf */
	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
sockflush(z_strm *strm)
{
	ssize_t slen;
	size_t len;
	char *p;
	int cnt;
	if (strm->obuflen == 0)
		return 0;
	p = strm->obuf + strm->obufpos;
	len = strm->obuflen - strm->obufpos;
	/* Flush stream, this may not succeed completely due
	 * to flow control and whatnot, which the docs suggest need
	 * resuming to complete.  So, use a loop, but to avoid
	 * getting endlessly stuck on this, only try a limited
	 * number of times. */
	for (cnt = 0; cnt < SERVER_MAX_SEND; cnt++) {
		if ((slen = write(strm->hdl.sock, p, len)) != len) {
			if (slen >= 0) {
				p += slen;
				len -= slen;
			} else if (errno != EINTR) {
				strm->obufpos = p - strm->obuf;
				return -1;
			}
		} else {
			strm->obufpos = 0;
			strm->obuflen = 0;
			return 0;
		}
	}
	/* so slow, throttle */
	errno = EAGAIN;
	return -1;
}

static inline int
sockclose(z_strm *strm)
{
	return close(strm->hdl.sock);
}

static void
sockfree(z_strm *strm)
{
	free(strm->obuf);
	free(strm);
}

static inline const char *
sockerror(z_strm *strm, int rval)
{
	(void)strm;
	(void)rval;
	return strerror(errno);
}

/* allocate new connection */
static int
socketnew(server *s, int osize) {
	s->strm = malloc(sizeof(z_strm));
	if (s->strm == NULL) {
		logerr("failed to alloc socket stream: out of memory\n");
		return -1;
	}
	s->strm->obufpos = 0;
	s->strm->obuflen = 0;
	s->strm->nextstrm = NULL;

	s->strm->obufsize = osize;

	s->strm->strmwrite = &sockwrite;
	s->strm->strmflush = &sockflush;
	s->strm->strmclose = &sockclose;
	s->strm->strmerror = &sockerror;
	s->strm->strmfree = &sockfree;

	s->strm->obuf = malloc(s->strm->obufsize);
	if (s->strm->obuf == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc socket stream buffer: out of memory\n");
		return -1;
	}

	return 0;
}

#ifdef HAVE_GZIP
/* gzip wrapped socket */
static inline int gzipflush(z_strm *strm);

static inline ssize_t
gzipwrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ) {
		/* need to flush */
		errno = ENOBUFS;
		return -1;
	}

	/* append metric to buf */
	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
gzipflush(z_strm *strm)
{
	int cret;
	char cbuf[METRIC_BUFSIZ];
	size_t cbuflen;
	char *cbufp;
	int oret;

	/* for non-blocking mode */
	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;

	if (strm->obuflen == 0)
		return 0;

	strm->hdl.gz->next_in = (Bytef *)strm->obuf;
	strm->hdl.gz->avail_in = strm->obuflen;
	strm->hdl.gz->next_out = (Bytef *)cbuf;
	strm->hdl.gz->avail_out = sizeof(cbuf);

	do {
		cret = deflate(strm->hdl.gz, Z_PARTIAL_FLUSH);
		if (cret == Z_OK && strm->hdl.gz->avail_out == 0) {
			/* too large output block, unlikely given the input, discard */
			/* TODO */
			break;
		}
		if (cret == Z_OK) {
			cbufp = cbuf;
			cbuflen = sizeof(cbuf) - strm->hdl.gz->avail_out;
			while (cbuflen > 0) {
				oret = strm->nextstrm->strmwrite(strm->nextstrm,
						cbufp, cbuflen);
				if (oret < 0)
					return -1;  /* failure is failure */

				/* update counters to possibly retry the remaining bit */
				cbufp += oret;
				cbuflen -= oret;
			}

			if (strm->hdl.gz->avail_in == 0)
				break;

			strm->hdl.gz->next_out = (Bytef *)cbuf;
			strm->hdl.gz->avail_out = sizeof(cbuf);
		} else {
			/* discard */
			break;
		}
	} while (1);
	if (cret != Z_OK)
		return -1;  /* we must reset/free gzip */

	/* flush whatever we wrote */
	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;

	/* reset the write position, from this point it will always need to
	 * restart */
	strm->obuflen = 0;

	return 0;
}

static inline int
gzipclose(z_strm *strm)
{
	int ret = strm->nextstrm->strmclose(strm->nextstrm);
	deflateEnd(strm->hdl.gz);
	return ret;
}

static inline void
gzipfree(z_strm *strm)
{
	strm->nextstrm->strmfree(strm->nextstrm);
	free(strm->hdl.gz);
	free(strm->obuf);
	free(strm);
}

static inline const char *
gziperror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
}

/* alloc on new server */
static int
gzipnew(server *s, int osize) {
	z_strm *gzstrm = malloc(sizeof(z_strm));
	if (gzstrm == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc gzip stream: out of memory\n");
		return -1;
	}
	gzstrm->strmwrite = &gzipwrite;
	gzstrm->strmflush = &gzipflush;
	gzstrm->strmclose = &gzipclose;
	gzstrm->strmerror = &gziperror;
	gzstrm->strmfree = &gzipfree;
	gzstrm->obufsize = osize;
	gzstrm->obuf = NULL;
	gzstrm->nextstrm = s->strm;

	s->strm = gzstrm;

	s->strm->hdl.gz = malloc(sizeof(z_stream));
	if (s->strm->hdl.gz == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc gzip stream: out of memory\n");
		return -1;
	}
	
	s->strm->obuf = malloc(s->strm->obufsize);
	if (s->strm->obuf == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc gzip stream buf: out of memory\n");
		return -1;
	}

	return 0;
}

/* setup on connect */
int
gzipsetup(server *s) {
	s->strm->obuflen = 0;
	s->strm->hdl.gz->zalloc = Z_NULL;
	s->strm->hdl.gz->zfree = Z_NULL;
	s->strm->hdl.gz->opaque = Z_NULL;
	s->strm->hdl.gz->next_in = Z_NULL;
	if (deflateInit2(s->strm->hdl.gz,
			Z_DEFAULT_COMPRESSION,
			Z_DEFLATED,
			15 + 16,
			8,
			Z_DEFAULT_STRATEGY) != Z_OK)
	{
		logerr("failed to setup gzip stream\n");
		return -1;
	}

	return 0;
}
#endif

#ifdef HAVE_LZ4

#include "lz4settings.h"

/* lz4 wrapped socket */
static inline int lzflush(z_strm *strm);

static inline ssize_t
lzwrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ) {
		/* need to flush */
		errno = ENOBUFS;
		return -1;
	}

	/* append metric to buf */
	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
lzflush(z_strm *strm)
{
	int oret;
	size_t ret;

		/* for non-blocking mode */
	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;

	if (strm->obuflen == 0)
		return 0;

	/* the buffered data goes out as a single frame */

	ret = LZ4F_compressFrame(strm->hdl.z.cbuf, strm->hdl.z.cbuflen, strm->obuf, strm->obuflen, NULL);
	if (LZ4F_isError(ret)) {
		logerr("Failed to compress %d LZ4 bytes into a frame: %d\n",
		       strm->obuflen, (int)ret);
		return -1;
	}
		/* write and flush */

	if ((oret = strm->nextstrm->strmwrite(strm->nextstrm,
					      strm->hdl.z.cbuf, ret)) < 0) {
		logerr("Failed to write %lu bytes of compressed LZ4 data: %d\n",
		       ret, oret);
		return oret;
	}

	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;

	/* the buffer is gone. reset the write position */

	strm->obuflen = 0;
	return 0;
}

static inline int
lzclose(z_strm *strm)
{
	return strm->nextstrm->strmclose(strm->nextstrm);
}

static void
lzfree(z_strm *strm)
{
	strm->nextstrm->strmfree(strm->nextstrm);	
	free(strm->hdl.z.cbuf);
	free(strm->obuf);
	free(strm);
}

static inline const char *
lzerror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
}

/* allocate new connection */
static int
lznew(server *s, int osize) {
	z_strm *lzstrm = malloc(sizeof(z_strm));
	if (lzstrm == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc lz4 stream: out of memory\n");
		return -1;
	}
	lzstrm->obufsize = osize;
	lzstrm->strmwrite = &lzwrite;
	lzstrm->strmflush = &lzflush;
	lzstrm->strmclose = &lzclose;
	lzstrm->strmerror = &lzerror;
	lzstrm->strmfree = &lzfree;
	lzstrm->nextstrm = s->strm;

	s->strm = lzstrm;

	s->strm->hdl.z.cbuf = NULL;
	s->strm->obuf = malloc(s->strm->obufsize);
	if (s->strm->obuf == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc gzip stream buf: out of memory\n");
		return -1;
	}

	/* get the maximum size that should ever be required and allocate for it */
	s->strm->hdl.z.cbuflen = LZ4F_compressBound(s->strm->obufsize, &lz4f_prefs);
	if ((s->strm->hdl.z.cbuf = malloc(s->strm->hdl.z.cbuflen)) == NULL) {
		s->strm->strmfree(s->strm);
		logerr("Failed to allocate %lu bytes for compressed LZ4 data\n", s->strm->hdl.z.cbuflen);
		return -1;
	}

	return 0;
}

/* setup on connect */
int
lzsetup(server *s) {
	s->strm->obuflen = 0;
	return 0;
}
#endif

#ifdef HAVE_SNAPPY
/* snappy wrapped socket */
static inline int snappyflush(z_strm *strm);

static inline ssize_t
snappywrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ)
		if (snappyflush(strm) != 0)
			return -1;

	/* append metric to buf */
	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
snappyflush(z_strm *strm)
{
	int cret;
	char cbuf[METRIC_BUFSIZ];
	size_t cbuflen = sizeof(cbuf);
	char *cbufp = cbuf;
	int oret;

	/* for non-blocking mode */
	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;

	if (strm->obuflen == 0)
		return 0;

	cret = snappy_compress(strm->obuf, strm->obuflen, cbuf, &cbuflen);

	if (cret != SNAPPY_OK)
		return -1;  /* we must reset/free snappy */
	while (cbuflen > 0) {
		oret = strm->nextstrm->strmwrite(strm->nextstrm, cbufp, cbuflen);
		if (oret < 0)
			return -1;  /* failure is failure */
		/* update counters to possibly retry the remaining bit */
		cbufp += oret;
		cbuflen -= oret;
	}
	if (strm->nextstrm->strmflush(strm->nextstrm) < 0)
		return -1;
	/* reset the write position, from this point it will always need to
	 * restart */
	strm->obuflen = 0;

	return 0;
}

static inline int
snappyclose(z_strm *strm)
{
	int ret = strm->nextstrm->strmclose(strm->nextstrm);
	return ret;
}

static void
snappyfree(z_strm *strm)
{
	strm->nextstrm->strmfree(strm->nextstrm);
	free(strm->obuf);
	free(strm);
}

static inline const char *
snappyerror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
}

/* allocate new connection */
static int
snappynew(server *s, int osize) {
	z_strm *snpstrm = malloc(sizeof(z_strm));
	if (snpstrm == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc snappy stream: out of memory\n");
		return -1;
	}
	snpstrm->strmwrite = &snappywrite;
	snpstrm->strmflush = &snappyflush;
	snpstrm->strmclose = &snappyclose;
	snpstrm->strmerror = &snappyerror;
	snpstrm->strmfree = &snappyfree;
	snpstrm->nextstrm = s->strm;
	snpstrm->obuflen = 0;

	s->strm = snpstrm;

	s->strm->obufsize = METRIC_BUFSIZ;
	s->strm->obuf = malloc(s->strm->obufsize);
	if (s->strm->obuf == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc gzip stream buf: out of memory\n");
		return -1;
	}

	return 0;
}

/* setup on connect */
int
snappysetup(server *s) {
	s->strm->obuflen = 0;
	return 0;
}
#endif

#ifdef HAVE_SSL
/* (Open|Libre)SSL wrapped socket */
static inline int sslflush(z_strm *strm);

static inline ssize_t
sslwrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ) {
		errno = ENOBUFS;
		return -1;
	}

	/* append metric to buf */
	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
sslflush(z_strm *strm)
{
	/* noop */
	ssize_t slen;
	size_t len;
	char *p;
	int cnt;
	if (strm->obuflen == 0)
		return 0;
	p = strm->obuf + strm->obufpos;
	len = strm->obuflen - strm->obufpos;
	for (cnt = 0; cnt < SERVER_MAX_SEND; cnt++) {
		errno = 0;
		if ((slen = SSL_write(strm->hdl.ssl, p, len)) != len) {
			if (slen >= 0) {
				p += slen;
				len -= slen;
			} else {
				int ssl_err;
				if (errno == EINTR)
					continue;

				strm->obufpos = p - strm->obuf;
				ssl_err = SSL_get_error(strm->hdl.ssl, slen);
				if (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE) {
					errno = EAGAIN;
				}
				return -1;

			}
		} else {
			strm->obuflen = 0;
			strm->obufpos = 0;
			return 0;
		}
	}

	/* so slow, throttle */
	errno = EAGAIN;
	return -1;
}

static inline int
sslclose(z_strm *strm)
{
	int sock = SSL_get_fd(strm->hdl.ssl);
	return close(sock);
}

static void
sslfree(z_strm *strm)
{
	strm->nextstrm->strmfree(strm->nextstrm);
	SSL_free(strm->hdl.ssl);
	SSL_CTX_free(strm->ctx);
	free(strm->obuf);
	free(strm);
}

static  __thread char _sslerror_buf[256];
static inline const char *
sslerror(z_strm *strm, int rval)
{
	int err = SSL_get_error(strm->hdl.ssl, rval);
	switch (err) {
		case SSL_ERROR_NONE:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: SSL_ERROR_NONE", err);
			break;
		case SSL_ERROR_ZERO_RETURN:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: TLS/SSL connection has been closed", err);
			break;
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_CONNECT:
		case SSL_ERROR_WANT_ACCEPT:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: the read or write operation did not complete", err);
			break;
		case SSL_ERROR_WANT_X509_LOOKUP:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: call callback via SSL_CTX_set_client_cert_cb()", err);
			break;
#ifdef SSL_ERROR_WANT_ASYNC
		case SSL_ERROR_WANT_ASYNC:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: asynchronous engine is still processing data", err);
			break;
#endif
#ifdef SSL_ERROR_WANT_ASYNC_JOB
		case SSL_ERROR_WANT_ASYNC_JOB:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: no async jobs available in the pool", err);
			break;
#endif
		case SSL_ERROR_SYSCALL:
			snprintf(_sslerror_buf, sizeof(_sslerror_buf),
					"%d: I/O error: %s", err, strerror(errno));
			break;
		case SSL_ERROR_SSL:
			ERR_error_string_n(ERR_get_error(),
					_sslerror_buf, sizeof(_sslerror_buf));
			break;
	}
	return _sslerror_buf;
}

static int
sslnew(server *s, int osize) {
	const SSL_METHOD *m = SSLv23_client_method();
	s->strm = malloc(sizeof(z_strm));
	if (s->strm == NULL) {
		logerr("failed to alloc SSL stream: out of memory\n");
		return -1;
	}
	s->strm->strmwrite = &sslwrite;
	s->strm->strmflush = &sslflush;
	s->strm->strmclose = &sslclose;
	s->strm->strmerror = &sslerror;
	s->strm->strmfree = &sslfree;

	s->strm->nextstrm = NULL;
	s->strm->obuf = NULL;
	s->strm->hdl.ssl = NULL;
	s->strm->ctx = SSL_CTX_new(m);

	if (sslCA != NULL) {
		if (s->strm->ctx == NULL ||
			SSL_CTX_load_verify_locations(s->strm->ctx,
											sslCAisdir ? NULL : sslCA,
											sslCAisdir ? sslCA : NULL) == 0)
		{
			char *err = ERR_error_string(ERR_get_error(), NULL);
			s->strm->strmfree(s->strm);
			logerr("failed to load SSL verify locations from %s for "
					"%s:%d: %s\n", sslCA, s->ip, s->port, err);
			return -1;
		}
	}
	SSL_CTX_set_verify(s->strm->ctx, SSL_VERIFY_PEER, NULL);

	s->strm->hdl.ssl = SSL_new(s->strm->ctx);
	if (s->strm->hdl.ssl == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc SSL stream: out of memory\n");
		return -1;
	}

	s->strm->obufsize = osize;
	s->strm->obuflen = 0;
	s->strm->obufpos = 0;

	s->strm->obuf = malloc(s->strm->obufsize);
	if (s->strm->obuf == NULL) {
		s->strm->strmfree(s->strm);
		logerr("failed to alloc socket stream buffer: out of memory\n");
		return -1;
	}

	return 0;
}
#endif

/* only for internal tests, not used in code */
z_strm *
server_get_strm(server *s)
{
	return s->strm;
}

static inline char server_add_failure(server *self) {
	char failure = __sync_fetch_and_add(&(self->failure), 1);
	if (failure > 100) {
		/* prevent char overflow */
		__sync_fetch_and_sub(&(self->failure), 20);
	}

	return failure;
}

/**
 * Returns unsended metrics back to the queue.
 */
static void
server_back_unsended(server *self, queue *q)
{
	if (self->len > 0) {
		size_t i;
		for (i = self->len - 1; i >= 0; i--) {
			const char *metric = self->batch[i];
			char ret = queue_putback(q, metric);
			if (ret == 0 && q != self->queue) {
				ret = queue_putback(self->queue, metric);
			}
			if (ret == 0) {
				if (mode & MODE_DEBUG)
					logerr("dropping metric: %s", *metric);
				free((char *) metric);
				__sync_add_and_fetch(&(self->dropped), 1);
			}
		}
		self->len = 0;
	}
}

int
server_disconnect(server *self) {
	if (self->fd != -1) {
		int ret = self->strm->strmclose(self->strm);
		server_back_unsended(self, self->queue);		
		self->fd = -1;
		return ret;
	} else {
		return 0;
	}
}

/** Returns true on success, or false if there was an error */
int socket_set_blocking(int fd, int blocking) {
   if (fd < 0) return 0;

   int flags = fcntl(fd, F_GETFL, 0);
   if (flags == -1) return -1;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? 1 : 0;
}

/* call after connect success */
int server_setup(server *self) {
#if defined(HAVE_SSL) || defined(HAVE_GZIP) || defined(HAVE_LZ4) || defined(HAVE_SNAPPY)
	int compress_type = self->transport & 0xFFFF;
#endif
	struct timeval timeout;
	/* ensure we will break out of connections being stuck more
	 * quickly than the kernel would give up */
	timeout.tv_sec = 10;
	timeout.tv_usec = (rand() % 300) * 1000;
	if (setsockopt(self->fd, SOL_SOCKET, SO_SNDTIMEO,
				&timeout, sizeof(timeout)) != 0)
		; /* ignore */
	if (self->sockbufsize > 0)
		if (setsockopt(self->fd, SOL_SOCKET, SO_SNDBUF,
					&self->sockbufsize, sizeof(self->sockbufsize)) != 0)
			; /* ignore */

#ifdef SO_NOSIGPIPE
	if (self->ctype == CON_TCP || self->ctype == CON_UDP) {
		int enable = 1;
		if (setsockopt(self->fd, SOL_SOCKET, SO_NOSIGPIPE,
					(void *)&enable, sizeof(enable)) != 0)
			logout("warning: failed to ignore SIGPIPE on socket: %s\n",
					strerror(errno));
	}
#endif

#ifdef HAVE_GZIP
	if (compress_type == W_GZIP) {
		if (gzipsetup(self) == -1) {
			server_add_failure(self);
			close(self->fd);
			self->fd = -1;
			return -1;
		}
	}
#endif
#ifdef HAVE_LZ4
	if (compress_type == W_LZ4) {
		if (lzsetup(self) == -1) {
			server_add_failure(self);
			close(self->fd);
			self->fd = -1;
			return -1;
		}
	}
#endif
#ifdef HAVE_SNAPPY
	if (compress_type == W_SNAPPY) {
		if (snappysetup(self) == -1) {
			server_add_failure(self);
			close(self->fd);
			self->fd = -1;
			return -1;
		}
	}
#endif
#ifdef HAVE_SSL
	if ((self->transport & ~0xFFFF) == W_SSL) {
		int rv;
		z_strm *sstrm;

		if (compress_type == W_PLAIN) { /* just SSL, nothing else */
			sstrm = self->strm;
		} else {
			sstrm = self->strm->nextstrm;
		}
		sstrm->obuflen = 0;

		/* make socket blocking again */
		if (socket_set_blocking(self->fd, 1) < 0) {
			server_add_failure(self);
			logerr("failed to remove socket non-blocking "
					"mode: %s\n", strerror(errno));
			close(self->fd);
			self->fd = -1;
			return -1;
		}
		SSL_set_tlsext_host_name(sstrm->hdl.ssl, self->ip);
		if (SSL_set_fd(sstrm->hdl.ssl, self->fd) == 0) {
			server_add_failure(self);
			logerr("failed to SSL_set_fd: %s\n",
					ERR_reason_error_string(ERR_get_error()));
			sstrm->strmclose(sstrm);
			self->fd = -1;
			return -1;
		}
		if ((rv = SSL_connect(sstrm->hdl.ssl)) != 1) {
			server_add_failure(self);
			logerr("failed to connect ssl stream: %s\n",
					sslerror(sstrm, rv));
			sstrm->strmclose(sstrm);
			self->fd = -1;
			return -1;
		}
		if ((rv = SSL_get_verify_result(sstrm->hdl.ssl)) != X509_V_OK) {
			server_add_failure(self);
			logerr("failed to verify ssl certificate: %s\n",
					sslerror(sstrm, rv));
			sstrm->strmclose(sstrm);
			self->fd = -1;
			return -1;
		}
		if (socket_set_blocking(self->fd, 0) < 0) {
			server_add_failure(self);
			logerr("failed to set socket non-blocking mode: %s\n",
					strerror(errno));
			close(self->fd);
			self->fd = -1;
			return -1;
		}
	} else
#endif
	{
		z_strm *pstrm;

		if (self->transport == W_PLAIN) { /* just plain socket */
			pstrm = self->strm;
		} else {
			pstrm = self->strm->nextstrm;
			pstrm->obuflen = 0;			
		}
		pstrm->hdl.sock = self->fd;
	}
	return self->fd;	
}

int server_connect(server *self)
{
	int args = 0;
	gettimeofday(&self->last, NULL);
	if (self->reresolve) {  /* can only be CON_UDP/CON_TCP */
		struct addrinfo *saddr;
		char sport[8];

		/* re-lookup the address info, if it fails, stay with
		 * whatever we have such that resolution errors incurred
		 * after starting the relay won't make it fail */
		freeaddrinfo(self->saddr);
		snprintf(sport, sizeof(sport), "%u", self->port);
		if (getaddrinfo(self->ip, sport, self->hint, &saddr) == 0) {
			self->saddr = saddr;
		} else {
			if (__sync_fetch_and_add(&(self->failure), 1) == 0)
				logerr("failed to resolve %s:%u, server unavailable\n",
						self->ip, self->port);
			self->saddr = NULL;
			/* this will break out below */
		}
	}

	if (self->ctype == CON_PIPE) {
		int intconn[2];
		connection *conn;
		if (pipe(intconn) < 0) {
			if (server_add_failure(self) == 0)
				logerr("failed to create pipe: %s\n", strerror(errno));
			return -1;
		}
		conn = dispatch_addconnection(intconn[0], NULL, dispatch_worker_with_low_connections(), 0, 1);
		if (conn == NULL) {
			if (server_add_failure(self) == 0)
				logerr("failed to add pipe: %s\n", strerror(errno));
			return -1;
		}
		self->fd = intconn[1];
	} else if (self->ctype == CON_FILE) {
		if ((self->fd = open(self->ip,
						O_WRONLY | O_APPEND | O_CREAT,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		{
			if (server_add_failure(self) == 0)
				logerr("failed to open file '%s': %s\n",
						self->ip, strerror(errno));
			return -1;
		}
	} else if (self->ctype == CON_UDP) {
		struct addrinfo *walk;

		for (walk = self->saddr; walk != NULL; walk = walk->ai_next) {
			if ((self->fd = socket(walk->ai_family,
							walk->ai_socktype,
							walk->ai_protocol)) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self) == 0)
					logerr("failed to create udp socket: %s\n",
							strerror(errno));
				return -1;
			}
			if (connect(self->fd, walk->ai_addr, walk->ai_addrlen) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self) == 0)
					logerr("failed to connect udp socket: %s\n",
							strerror(errno));
				close(self->fd);
				self->fd = -1;
				return -1;
			}

			/* we made it up here, so this connection is usable */
			break;
		}
		/* if this didn't resolve to anything, treat as failure */
		if (self->saddr == NULL)
			server_add_failure(self);
		/* if all addrinfos failed, try again later */
		if (self->fd < 0)
			return -1;
	} else {  /* CON_TCP */
		int ret;
		struct addrinfo *walk;

		for (walk = self->saddr; walk != NULL; walk = walk->ai_next) {
			if ((self->fd = socket(walk->ai_family,
							walk->ai_socktype,
							walk->ai_protocol)) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self) == 0)
					logerr("failed to create socket: %s\n",
							strerror(errno));
				return -1;
			}

			/* put socket in non-blocking mode such that we can
			 * poll() (time-out) on the connect() call */
			if (socket_set_blocking(self->fd, 0) < 0) {
				server_add_failure(self);
				logerr("failed to set socket non-blocking mode: %s\n",
						strerror(errno));
				close(self->fd);
				self->fd = -1;
				return -1;
			}
			ret = connect(self->fd, walk->ai_addr, walk->ai_addrlen);

			if (ret < 0 && errno == EINPROGRESS) {
				/* TODO - skip, wait in queueread */
				/* wait for connection to succeed if the OS thinks
				 * it can successed */
				struct pollfd ufds[1];
				ufds[0].fd = self->fd;
				ufds[0].events = POLLIN | POLLOUT;
				ret = poll(ufds, 1, self->iotimeout + (rand() % 100));
				if (ret == 0) {
					/* time limit expired */
					if (walk->ai_next == NULL &&
							server_add_failure(self) == 0)
						logerr("failed to connect() to "
								"%s:%u: Operation timed out\n",
								self->ip, self->port);
					close(self->fd);
					self->fd = -1;
					return -1;
				} else if (ret < 0) {
					/* some select error occurred */
					if (walk->ai_next &&
							server_add_failure(self) == 0)
						logerr("failed to poll() for %s:%u: %s\n",
								self->ip, self->port, strerror(errno));
					close(self->fd);
					self->fd = -1;
					return -1;
				} else {
					if (ufds[0].revents & POLLHUP) {
						if (walk->ai_next == NULL &&
								server_add_failure(self) == 0)
							logerr("failed to connect() for %s:%u: "
									"Connection refused\n",
									self->ip, self->port);
						close(self->fd);
						self->fd = -1;
						return -1;
					}
					/*
					int valopt;
					socklen_t lon = sizeof(int);
					if (getsockopt(self->fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
						logerr("failed in getsockopt() %s:%u: %s\n", self->ip, self->port, strerror(errno));
						continue;
					}
					if (valopt) {
					   logerr("failed delayed connect() %s:%d - %s\n", self->ip, self->port, strerror(valopt));
					   exit(0);
					}
					*/
				}
			} else if (ret < 0) {
				if (walk->ai_next == NULL &&
						server_add_failure(self) == 0)
				{
					logerr("failed to connect() to %s:%u: %s\n",
							self->ip, self->port, strerror(errno));
					dispatch_check_rlimit_and_warn();
				}
				close(self->fd);
				self->fd = -1;
				return -1;
			}

			/* make socket blocking again */
			// if (fcntl(self->fd, F_SETFL, args) < 0) {
			// 	logerr("failed to remove socket non-blocking "
			// 			"mode: %s\n", strerror(errno));
			// 	close(self->fd);
			// 	self->fd = -1;
			// 	return -1;
			// }

			/* disable Nagle's algorithm, issue #208 */
			args = 1;
			if (setsockopt(self->fd, IPPROTO_TCP, TCP_NODELAY,
						&args, sizeof(args)) != 0)
				; /* ignore */

#ifdef TCP_USER_TIMEOUT
			/* break out of connections when no ACK is being
			 * received for +- 10 seconds instead of
			 * retransmitting for +- 15 minutes available on
			 * linux >= 2.6.37
			 * the 10 seconds is in line with the SO_SNDTIMEO
			 * set on the socket below */
			args = 10000 + (rand() % 300);
			if (setsockopt(self->fd, IPPROTO_TCP, TCP_USER_TIMEOUT,
						&args, sizeof(args)) != 0)
				; /* ignore */
#endif

			/* if we reached up here, we're good to go, so don't
			 * continue with the other addrinfos */
			break;
		}
		/* if this didn't resolve to anything, treat as failure */
		if (self->saddr == NULL)
			server_add_failure(self);
		/* all available addrinfos failed on us */
		if (self->fd < 0)
			return -1;
	}

	return server_setup(self);
}

queue *server_queue(server *s)
{
	return s->queue;
}

char server_queue_is_shared(server *s)
{
	return s->shared_queue;
}

queue *server_set_shared_queue(server *s, queue *q)
{
	if (s->shared_queue && s->queue != q) {
		queue *qold = s->queue;
		s->queue = q;
		return qold;
	}
	return NULL;
}

static inline int server_secpos_alloc(server *self)
{
	if (self->secondariescnt > 0) {
		int i;		
		self->secpos = malloc(sizeof(size_t) * self->secondariescnt);
		if (self->secpos == NULL) {
			logerr("server: failed to allocate memory "
					"for secpos\n");
			return -1;
		}
		for (i = 0; i < self->secondariescnt; i++)
			self->secpos[i] = i;
	}
	return 0;
}


/* check for write ready 
 * TODO: replace with poll on serveral servers fd in cluster threads for reduce cpu usage */
int server_poll(server *self) {
	int ret;
	struct pollfd ufd;

	ufd.fd = self->fd;
	ufd.events = POLLOUT;
	ret = poll(&ufd, 1, 10000);
	if (ret == 0) {
		/* timeout */
		logerr("failed to write %s:%u, server timeout\n", self->ip, self->port);
		server_disconnect(self);
	} else if (ret == -1) {
		logerr("failed to write %s:%u, server poll error: %s\n", self->ip, self->port, strerror(errno));
		server_disconnect(self);
	} else if (ufd.revents & POLLHUP) {
		logerr("failed to write %s:%u, server connection hungup\n", self->ip, self->port);
		server_disconnect(self);
	} else if (!(ufd.revents & POLLOUT)) {
		logerr("failed to write %s:%u, server poll revents: %d\n", self->ip, self->port, ufd.revents);
		server_disconnect(self);
	}
	return ret;
}

static ssize_t server_queueread(server *self, queue *q, struct pollfd *ufd, char keep_running)
{
	ssize_t slen;
	const char *metric, *m;
	size_t len, mlen;
	struct timeval start, stop;

	if (self->fd == -1) {
		errno = ENOTCONN;
		return -1;
	}

	gettimeofday(&start, NULL);
	if (ufd) {
		if (ufd->revents & POLLHUP) {
			logerr("failed to write %s:%u, server connection hungup\n",
					self->ip, self->port);
			server_disconnect(self);
			return -1;
		} else if (!(ufd->revents & POLLOUT)) {
			logerr("failed to write %s:%u, server poll revents: %d\n",
					self->ip, self->port, ufd->revents);
			server_disconnect(self);
			return -1;
		} else if (ufd->revents == 0) {
			__sync_add_and_fetch(&(self->waits), timediff(self->last, start));
			self->last = start;
			errno = EAGAIN;
			return -1;
		}
	}
	
	if (keep_running != SERVER_KEEP_RUNNING) {
		/* be noisy during shutdown so we can track any slowing down
			* servers, possibly preventing us to shut down */
		size_t l = queue_len(q);
		if (l > 0) {
			logerr("shutting down %s:%u: waiting for %zu metrics\n",
					self->ip, self->port, l);
		}
	}

	if (self->len == 0) {
		while (self->len < self->bsize) {
			metric = queue_dequeue(q);
			if (metric == NULL) {
				break;
			}
			mlen = *(size_t *)metric;
			m = metric + sizeof(size_t);
			self->batch[self->len] = metric;
			self->len++;
			slen = self->strm->strmwrite(self->strm, m, mlen);
			if (slen == -1 && (errno == ENOBUFS || errno == EAGAIN || errno == EWOULDBLOCK)) {
				/* flush and return last message back to the queue */
				if (queue_putback(q, metric) == 0) {
					if (mode & MODE_DEBUG)
						logerr("dropping metric: %s", *metric);
					free((char *) metric);
					__sync_add_and_fetch(&(self->dropped), 1);
				}
				break;
			} else if (slen != mlen) {
				/* not fully sent (after tries), or failure
				 * close connection regardless so we don't get
				 * synchonisation problems */
				if (self->ctype != CON_UDP &&
						server_add_failure(self) == 0)
					logerr("failed to write() to %s:%u: %s\n",
							self->ip, self->port,
							(slen < 0 ?
								self->strm->strmerror(self->strm, slen) :
								"incomplete write"));
				server_disconnect(self);
			}
		}
	}

	if (self->len == 0) {
		self->last = start;
		return 0;
	}

	if (self->fd != -1) {
		if (self->strm->strmflush(self->strm) == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				return -1;
			}
			if (self->ctype != CON_UDP &&
					server_add_failure(self) == 0)
						logerr("failed to flush() to %s:%u: %s\n",
								self->ip, self->port,
								(slen < 0 ?
									self->strm->strmerror(self->strm, slen) :
									"flush error"));
			server_disconnect(self);
		} else {
			if (!__sync_bool_compare_and_swap(&(self->failure), 0, 0)) {
				if (self->ctype != CON_UDP)
					logerr("server %s:%u: OK\n", self->ip, self->port);
				__sync_and_and_fetch(&(self->failure), 0);
			}
			__sync_add_and_fetch(&(self->metrics), self->len);
			len = self->len;
			self->len = 0;
		}
	}

	gettimeofday(&stop, NULL);
	__sync_add_and_fetch(&(self->ticks), timediff(start, stop));
	self->last = start;

	if (self->fd == -1) {
		server_back_unsended(self, q);
		return -1;
	} else {
		return len;
	}
}

static struct pollfd *pollfd_find(struct pollfd *ufds, int count, int *cur_pos, int fd)
{
	if (fd == -1) {
		return NULL;
	}
	while (*cur_pos < count) {
		if (fd == ufds[*cur_pos].fd) {
			return &ufds[*cur_pos];
		}
#pragma GCC diagnostic ignored "-Wunused-value"
		*cur_pos++;
#pragma GCC diagnostic pop
	}
	*cur_pos = 0;
	return NULL;
}

static void *
cluster_queuereader(void *d)
{
	cluster *self = (cluster *) d;

	ssize_t stimeout = shutdown_timeout * 1000000; /*shutdown timeout in microseconds */
	char keep_running;
	size_t n;
	struct pollfd ufds[256];
	int ret = 0, iotimeout = 600; /* connection timeout */
	struct timeval start, stop;

	start.tv_sec = 0;
	start.tv_usec = 0;

	servers *ss = cluster_servers(self);
	if (ss->server != NULL)
		iotimeout = ss->server->iotimeout;

	while (1) {
		servers *s;

        keep_running = __sync_add_and_fetch(&(self->keep_running), 0);
		if (keep_running == SERVER_TRY_SEND) {
			/* check shutdown timeout */
			if (start.tv_sec == 0 && start.tv_usec == 0) {
				/* shutdown initiated */
				gettimeofday(&start, NULL);
			} else {
				gettimeofday(&stop, NULL);
				if (timediff(start, stop) >= stimeout) {
					break;
				}
				/* check for emthy queue */
				size_t qlen = 0;
				for (s = ss; s != NULL; s = s->next) {
					qlen += queue_len(s->server->queue) + s->server->len;
				}
				if (qlen == 0) {
					break;
				}
			}
		} else if (keep_running != SERVER_KEEP_RUNNING) {
			/* force shutdown without send */
			break;
		}

		n = 0;
		for (s = ss; s != NULL; s = s->next) {
			if (s->server->fd != -1) {
				ufds[n].fd = s->server->fd;
				ufds[n].events = POLLOUT;
				n++;
			}
		}
		if (n > 0)
			ret = poll(ufds, n, iotimeout);

		if (ret == -1) {
			logerr(
				"failed to write %s, cluster servers poll error: %s\n",
				self->name, strerror(errno));
			for (s = ss; s != NULL; s = s->next) {
				if (s->server->fd > -1) {
					server_disconnect(s->server);
				}
			}
		} else if (ret >= 0) {
			if (self->type == FAILOVER) {
				int overload, i = 0;
				size_t qfree = queue_free(self->queue);
				if (keep_running == SERVER_TRY_SEND) {
					overload = 1;
				} else {
					overload = QUEUE_FREE_CRITICAL(qfree, ss->server);
				}
				if (ss->server->qfree_threshold != queuefree_threshold_start && ! overload) {
					/* threshold for cancel rebalance */
					ss->server->qfree_threshold = queuefree_threshold_start;
					if (mode && MODE_DEBUG)
						tracef("throttle end %s:%u: waiting for %zu metrics\n",
								ss->server->ip, ss->server->port, queue_len(self->queue));
				} else if (ss->server->qfree_threshold == queuefree_threshold_start && overload) {
					/* destination overloaded, set threshold for destination recovery */
					ss->server->qfree_threshold = queuefree_threshold_end;
					if (mode && MODE_DEBUG)
						tracef("throttle %s:%u: waiting for %zu metrics\n",
								ss->server->ip, ss->server->port, queue_len(self->queue));
				}		
				for (s = ss; s != NULL; s = s->next) {
					struct pollfd *p = pollfd_find(ufds, n, &i, s->server->fd);
					ssize_t len = server_queueread(s->server, self->queue, p, keep_running);
					if (len == 0) {
						break;
					} else if (len > 0) {
						if (s != ss) {
							__sync_add_and_fetch(&(ss->server->requeue), (size_t) len);
						}
						if (!overload) /* check for overload */
							break;
					} if (s->server->fd == -1) {
						server_connect(s->server);
					} else if (errno != EAGAIN && errno != EWOULDBLOCK) {
						char failure = __sync_fetch_and_add(&(s->server->failure), 1);
						if (failure < FAIL_WAIT_COUNT && !overload)
							break;
					}
				}
			} else {
				queue *q = NULL;				
				server *s_min = NULL;
				int i = 0;				
				if (self->type == ANYOF || self->isdynamic) {
					int overload;
					size_t qfree_min = 0;
									
					for (s = ss; s != NULL; s = s->next) {
						size_t qfree = queue_free(s->server->queue);
						if (q == NULL || qfree < qfree_min) {
							s_min = s->server;
							qfree_min = qfree;
						}
						overload = QUEUE_FREE_CRITICAL(qfree, s->server);
						if (s->server->qfree_threshold != queuefree_threshold_start && ! overload) {
							/* threshold for cancel rebalance */
							s->server->qfree_threshold = queuefree_threshold_start;
							if (mode && MODE_DEBUG)
								tracef("throttle end %s:%u: waiting for %zu metrics\n",
										s->server->ip, s->server->port, queue_len(s->server->queue));
						} else if (s->server->qfree_threshold == queuefree_threshold_start && overload) {
							/* destination overloaded, set threshold for destination recovery */
							s->server->qfree_threshold = queuefree_threshold_end;
							if (mode && MODE_DEBUG)
								tracef("throttle %s:%u: waiting for %zu metrics\n",
										s->server->ip, s->server->port, queue_len(s->server->queue));
						}
					}
					overload = QUEUE_FREE_CRITICAL(qfree_min, s_min);
					if (!overload) {
						q = NULL;
					}
				}

				for (s = ss; s != NULL; s = s->next) {
					struct pollfd *p = pollfd_find(ufds, n, &i, s->server->fd);
					ssize_t len;
					if (q) {
						len = server_queueread(s->server, q, p, keep_running);
						if (len > 0 && s->server != s_min)
							__sync_add_and_fetch(&(s_min->requeue), (size_t) len);
					} else {
						len = server_queueread(s->server, s->server->queue, p, keep_running);
					}
					if (len < 0) {
						if (s->server->fd == -1) {
							server_connect(s->server);
						}
					}
				}
			}
		}
	}

	__sync_bool_compare_and_swap(&(self->running), 1, 0);
	return NULL;
}

/**
 * Reads from the queue and sends items to the remote server.  This
 * function is designed to be a thread.  Data sending is attempted to be
 * batched, but sent one by one to reduce loss on sending failure.
 * A connection with the server is maintained for as long as there is
 * data to be written.  As soon as there is none, the connection is
 * dropped if a timeout of DISCONNECT_WAIT_TIME exceeds.
 * ONLY for used without cluster (internal submission)
 */
static void *
server_queuereader(void *d)
{
	server *self = (server *)d;

	ssize_t len = -1;
	struct timeval start, stop;

	char keep_running = SERVER_KEEP_RUNNING;
	ssize_t stimeout = shutdown_timeout * 1000000; /*shutdown timeout in microseconds */

	self->running = 1;
	self->alive = 1;

	while (1) {
		keep_running = __sync_add_and_fetch(&(self->keep_running), 0);
		if (keep_running == SERVER_TRY_SEND) {
			/* check shutdown timeout */
			if (len == 0)
				break;
			if (start.tv_sec == 0 && start.tv_usec == 0) {
				gettimeofday(&start, NULL);
			} else {
				gettimeofday(&stop, NULL);
				if (timediff(start, stop) >= stimeout) {
					break;
				}
				if (queue_len(self->queue) + self->len == 0)
					break;
			}
		} else if (keep_running != SERVER_KEEP_RUNNING) {
			break;
		}

		if (self->fd == -1) {
			server_connect(self);
		} else {
			len = server_queueread(self, self->queue, NULL, keep_running);
		}
	}

	__sync_bool_compare_and_swap(&(self->alive), 1, 0);
	logout("shut down %s:%d\n", self->ip, self->port);
	if (self->fd >= 0) {
		self->strm->strmflush(self->strm);
		server_disconnect(self);
	}

	__sync_bool_compare_and_swap(&(self->running), 1, 0);
	return NULL;
}

void server_cleanup(server *s);


/**
 * Allocate a new (outbound) server.  Effectively this means a thread
 * that reads from the queue and sends this as good as it can to the ip
 * address and port associated.
 */
server *
server_new(
		const char *ip,
		unsigned short port,
		con_type type,
		con_trnsp transport,
		con_proto ctype,
		struct addrinfo *saddr,
		struct addrinfo *hint,
		size_t qsize,
		queue *q,
		size_t bsize,
		int maxstalls,
		unsigned short iotimeout,
		unsigned int sockbufsize)
{
	server *ret;

	if (q == NULL && qsize == 0) {
		return NULL;
	}

	if ((ret = malloc(sizeof(server))) == NULL)
		return NULL;

	ret->secpos = NULL;
	ret->type = type;
	ret->transport = transport;
	ret->ctype = ctype;
	ret->tid = 0;
	ret->secpos = NULL;
	ret->secondaries = NULL;
	ret->secondariescnt = 0;
	ret->len = 0;
	ret->ip = strdup(ip);
	if (ret->ip == NULL) {
		free(ret);
		return NULL;
	}
	ret->queue = NULL;
	ret->saddr = NULL;
	ret->hint = NULL;
	ret->strm = NULL;
	ret->secpos = NULL;
	ret->port = port;
	ret->instance = NULL;
	ret->bsize = bsize;
	ret->qfree_threshold = 2 * bsize;
	ret->iotimeout = iotimeout < 250 ? 600 : iotimeout;
	ret->sockbufsize = sockbufsize;
	ret->maxstalls = maxstalls;
	if ((ret->batch = malloc(sizeof(char *) * (bsize + 1))) == NULL) {
		server_cleanup(ret);
		return NULL;
	}
	ret->fd = -1;

	if (q == NULL) {
		ret->shared_queue = 0;
		ret->queue = queue_new(qsize);
		if (ret->queue == NULL) {
			server_cleanup(ret);
			return NULL;
		}
	} else {
		ret->shared_queue = 1;
		ret->queue = q;
	}

	/* setup normal or SSL-wrapped socket first */
#ifdef HAVE_SSL
	if ((transport & ~0xFFFF) == W_SSL) {
		/* create an auto-negotiate context */
		if (sslnew(ret, METRIC_BUFSIZ) == -1) {
			server_cleanup(ret);
			return NULL;
		}
	} else
#endif
	{
		if (socketnew(ret, METRIC_BUFSIZ) == -1) {
			server_cleanup(ret);
			return NULL;
		}
	}
	
	/* now see if we have a compressor defined */
	if ((transport & 0xFFFF) == W_PLAIN) {
		/* catch noop */
	}
#ifdef HAVE_GZIP
	else if ((transport & 0xFFFF) == W_GZIP) {
		if (gzipnew(ret, METRIC_BUFSIZ) == -1) {
			server_cleanup(ret);
			return NULL;
		}
	}
#endif
#ifdef HAVE_LZ4
	else if ((transport & 0xFFFF) == W_LZ4) {
		if (lznew(ret, METRIC_BUFSIZ) == -1) {
			server_cleanup(ret);
			return NULL;
		}
	}
#endif
#ifdef HAVE_SNAPPY
	else if ((transport & 0xFFFF) == W_SNAPPY) {
		if (snappynew(ret, METRIC_BUFSIZ) == -1) {
			server_cleanup(ret);
			return NULL;
		}
	}
#endif

	ret->saddr = saddr;
	ret->reresolve = 0;
	if (hint != NULL) {
		ret->reresolve = 1;
		ret->hint = hint;
	}

	ret->last.tv_sec = 0;
	ret->last.tv_usec = 0;
	ret->failover = 0;
	ret->failure = 0;
	ret->running = 0;
	ret->keep_running = SERVER_KEEP_RUNNING;
	ret->stallseq = 0;
	ret->metrics = 0;
	ret->dropped = 0;
	ret->requeue = 0;
	ret->stalls = 0;
	ret->ticks = 0;
	ret->waits = 0;
	ret->prevmetrics = 0;
	ret->prevdropped = 0;
	ret->prevrequeue = 0;
	ret->prevstalls = 0;
	ret->prevticks = 0;
	ret->prevwaits = 0;
	ret->tid = 0;

	return ret;
}

/**
 * Compare server s against the address info in saddr.  A server is
 * considered to be equal is it is of the same socket family, type and
 * protocol, and if the target address and port are the same.  When
 * saddr is NULL, a match against the given ip is attempted, e.g. for
 * file destinations.
 */
char
server_cmp(server *s, struct addrinfo *saddr, const char *ip, unsigned short port, con_proto proto)
{
	if ((saddr == NULL || s->saddr == NULL)) {
		if (strcmp(s->ip, ip) == 0 && s->port == port && s->ctype == proto)
			return 0;
	} else if (
			s->saddr->ai_family == saddr->ai_family &&
			s->saddr->ai_socktype == saddr->ai_socktype &&
			s->saddr->ai_protocol == saddr->ai_protocol
	   )
	{
		if (saddr->ai_family == AF_INET) {
			struct sockaddr_in *l = ((struct sockaddr_in *)s->saddr->ai_addr);
			struct sockaddr_in *r = ((struct sockaddr_in *)saddr->ai_addr);
			if (l->sin_port == r->sin_port &&
					l->sin_addr.s_addr == r->sin_addr.s_addr)
				return 0;
		} else if (saddr->ai_family == AF_INET6) {
			struct sockaddr_in6 *l =
				((struct sockaddr_in6 *)s->saddr->ai_addr);
			struct sockaddr_in6 *r =
				((struct sockaddr_in6 *)saddr->ai_addr);
			if (l->sin6_port == r->sin6_port &&
					memcmp(l->sin6_addr.s6_addr, r->sin6_addr.s6_addr,
						sizeof(l->sin6_addr.s6_addr)) == 0)
				return 0;
		}
	}

	return 1;  /* not equal */
}

servers *cluster_servers(cluster *c)
{
	switch (c->type) {
		case FORWARD:
		case FILELOG:
		case FILELOGIP:
			return c->members.forward;
			;;
		case FAILOVER:		
		case ANYOF:
			return c->members.anyof->list;
			;;
		case CARBON_CH:
		case FNV1A_CH:
		case JUMP_CH:
			return c->members.ch->servers;
			;;
		default:
			return NULL;
			;;
	}
}

/* Start cluster servers */
char cluster_start(cluster *c)
{
	int err = 0;
	servers *s = cluster_servers(c);

	if (s == NULL) {
		return 0;
	}

	for (; s != NULL; s = s->next) {
		if (server_secpos_alloc(s->server) == -1) {
			logerr("failed to alloc server secpos %s:%u\n",
					server_ip(s->server),
					server_port(s->server));
			err = 1;
			return err;
		}
	}

	// for (s = cluster_servers(c); s != NULL; s = s->next) {
	// 	if ((err = server_start(s->server)) != 0) {
	// 		logerr("failed to start server %s:%u: %s\n",
	// 				server_ip(s->server),
	// 				server_port(s->server),
	// 				strerror(err));
	// 		err = 1;
	// 		break;
	// 	}
	// }
	// 
	// return err;

	return pthread_create(&c->tid, NULL, &cluster_queuereader, c);
}

/**
 * Starts a previously created server using server_new().  Returns
 * errno if starting a thread failed, after which the caller should
 * server_free() the given s pointer.
 */
char
server_start(server *s)
{
	return pthread_create(&s->tid, NULL, &server_queuereader, s);
}

/**
 * Adds a list of secondary servers to this server.  A secondary server
 * is a server which' queue will be checked when this server has nothing
 * to do.
 */
void
server_add_secondaries(server *self, server **secondaries, size_t count)
{
	self->secondaries = secondaries;
	self->secondariescnt = count;
}

/**
 * Flags this server as part of a failover cluster, which means the
 * secondaries are used only to offload on failure, not on queue stress.
 */
void
server_set_failover(server *self)
{
	self->failover = 1;
}

/**
 * Sets instance name only used for carbon_ch cluster type.
 */
void
server_set_instance(server *self, char *instance)
{
	if (self->instance != NULL)
		free(self->instance);
	self->instance = strdup(instance);
}

/**
 * Thin wrapper around the associated queue with the server object.
 * Returns true if the metric could be queued for sending, or the metric
 * was dropped because the associated server is down.  Returns false
 * otherwise (when a retry seems like it could succeed shortly).
 */
inline char
server_send(server *s, const char *d, char force)
{
	size_t qfree = queue_free(s->queue);
	if (!s->shared_queue) {
		/* check for overloaded destination */
		if (!force && s->secondariescnt > 0 &&
			(__sync_add_and_fetch(&(s->failure), 0) || QUEUE_FREE_CRITICAL(qfree, s))) {
			size_t i;
			size_t maxqfree = qfree + 4 * s->bsize;
			queue *squeue = s->queue;

			for (i = 0; i < s->secondariescnt; i++) {
				if (s != s->secondaries[i] && !__sync_add_and_fetch(&(s->secondaries[i]->failure), 0) &&
						__sync_bool_compare_and_swap(&(s->secondaries[i]->keep_running), 1, 1)) {
					size_t sqfree = queue_free(s->secondaries[i]->queue);
					if (sqfree >  maxqfree) {
						squeue = s->secondaries[i]->queue;
						maxqfree = sqfree;
					}
				}
			}
			if (squeue != s->queue) {
				if (__sync_add_and_fetch(&(s->failure), 0) == 0)
					__sync_add_and_fetch(&(s->requeue), 1);
				queue_enqueue(squeue, d);
				return 1;
			}
		}
	}
	if (qfree == 0) {
		char failure = __sync_add_and_fetch(&(s->failure), 0);
		if (!s->shared_queue && !force && s->secondariescnt > 0) {
			size_t i;
			/* don't immediately drop if we know there are others that
			 * back us up */
			for (i = 0; i < s->secondariescnt; i++) {
				if (!__sync_add_and_fetch(&(s->secondaries[i]->failure), 0)) {
					failure = 0;
					break;
				}
			}
		}
		if (failure || force ||
			__sync_add_and_fetch(&(s->stallseq), 0) == s->maxstalls)
		{
			__sync_add_and_fetch(&(s->dropped), 1);
			/* excess event will be dropped by the enqueue below */
		} else {
			__sync_add_and_fetch(&(s->stallseq), 1);
			__sync_add_and_fetch(&(s->stalls), 1);
			return 0;
		}
	} else {
		__sync_and_and_fetch(&(s->stallseq), 0);
	}
	queue_enqueue(s->queue, d);

	return 1;
}

/* Iniciate shutdown cluster servers */
void cluster_shutdown(cluster *c, int swap)
{
	servers *s = cluster_servers(c);

	for ( ; s != NULL; s = s->next) {
		server_shutdown(s->server, swap);
	}

	if (swap) {
		__sync_bool_compare_and_swap(&(c->keep_running), SERVER_KEEP_RUNNING, SERVER_TRY_SWAP);
	} else {
		__sync_bool_compare_and_swap(&(c->keep_running), SERVER_KEEP_RUNNING, SERVER_TRY_SEND);
	}
}

void cluster_shutdown_wait(cluster *c)
{
	int err;
	servers *s = cluster_servers(c);

	for ( ; s != NULL; s = s->next) {
		server_shutdown_wait(s->server);
	}

	if (c->tid != 0 && (err = pthread_join(c->tid, NULL)) != 0)
		logerr("%s: failed to join cluster thread: %s\n",
				c->name, strerror(err));
	c->tid = 0;
}

/**
 * Tells this server to finish sending pending items from its queue.
 */
void
server_shutdown(server *s, int swap)
{
	if (swap) {
		__sync_bool_compare_and_swap(&(s->keep_running), SERVER_KEEP_RUNNING, SERVER_TRY_SWAP);
	} else {
		__sync_bool_compare_and_swap(&(s->keep_running), SERVER_KEEP_RUNNING, SERVER_TRY_SEND);
	}
}

void server_shutdown_wait(server *s)
{
	int i, err;
	/* wait for the secondaries to be stopped so we surely don't get
     * invalid reads when server_free is called */
	if (s->secondariescnt > 0) {
		for (i = 0; i < s->secondariescnt; i++) {
		   while (!__sync_bool_compare_and_swap(&(s->secondaries[i]->running), 0, 0))
			   usleep(200 * 1000);
		}
	} else {
	   while (!__sync_bool_compare_and_swap(&(s->running), 0, 0))
		   usleep(200 * 1000);
	}
	if (s->tid != 0 && (err = pthread_join(s->tid, NULL)) != 0)
		logerr("%s:%u: failed to join server thread: %s\n",
				s->ip, s->port, strerror(err));
	s->tid = 0;
}

/**
 * Frees this server and associated resources without joining thread
 */
void
server_cleanup(server *s) {
	if (s->queue != NULL && s->shared_queue == 0) {
		queue_destroy(s->queue);
	}
	free(s->batch);
	free(s->instance);
	freeaddrinfo(s->saddr);
	freeaddrinfo(s->hint);
	if (s->strm != NULL) {
		s->strm->strmfree(s->strm);
	}
	free((char *)s->ip);
	s->ip = NULL;
	free(s->secpos);
	free((char *)s->ip);
	free(s);
}
/**
 * Frees this server and associated resources.  This includes joining
 * the server thread.
 */
void
server_free(server *s) {
	int err;

	if (s->tid != 0 && (err = pthread_join(s->tid, NULL)) != 0)
		logerr("%s:%u: failed to join server thread: %s\n",
				s->ip, s->port, strerror(err));
	s->tid = 0;

	if (s->ctype == CON_TCP) {
		size_t qlen = queue_len(s->queue);
		if (qlen > 0)
			logerr("dropping %zu metrics for %s:%u\n",
					qlen, s->ip, s->port);
	}

	server_cleanup(s);
}

/**
 * Swaps the queue between server l to r.  This is assumes both l and r
 * are not running.
 */
void
server_swap_queue(server *l, server *r)
{
	queue *t;
	char shared;

	assert(l->tid == 0);
	assert(r->tid == 0);

	t = l->queue;
	shared = l->shared_queue;
	l->queue = r->queue;
	l->shared_queue = r->shared_queue;
	r->queue = t;
	r->shared_queue = shared;
	
	/* swap associated statistics as well */
	l->metrics = r->metrics;
	l->dropped = r->dropped;
	l->stalls = r->stalls;
	l->ticks = r->ticks;
	l->waits = r->waits;
	l->requeue = r->requeue;
	l->prevmetrics = r->prevmetrics;
	l->prevdropped = r->prevdropped;
	l->prevstalls = r->prevstalls;
	l->prevticks = r->prevticks;
	l->prevwaits = r->prevwaits;
	l->prevrequeue = r->prevrequeue;
}

/**
 * Returns the ip address this server points to.
 */
inline const char *
server_ip(server *s)
{
	if (s == NULL)
		return NULL;
	return s->ip;
}

/**
 * Returns the port this server connects at.
 */
inline unsigned short
server_port(server *s)
{
	if (s == NULL)
		return 0;
	return s->port;
}

/**
 * Returns the instance associated with this server.
 */
inline char *
server_instance(server *s)
{
	return s->instance;
}

/**
 * Returns the connection protocol of this server.
 */
inline con_proto
server_ctype(server *s)
{
	if (s == NULL)
		return CON_PIPE;
	return s->ctype;
}

/**
 * Returns the connection transport of this server.
 */
inline con_trnsp
server_transport(server *s)
{
	if (s == NULL)
		return W_PLAIN;
	return s->transport;
}

/**
 * Returns the connection type of this server.
 */
inline con_type
server_type(server *s)
{
	if (s == NULL)
		return T_LINEMODE;
	return s->type;
}

/**
 * Returns whether the last action on this server caused a failure.
 */
inline char
server_failed(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->failure), 0);
}

/**
 * Returns the wall-clock time in microseconds (us) consumed sending metrics.
 */
inline size_t
server_get_ticks(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->ticks), 0);
}

/**
 * Returns the wall-clock time in microseconds (us) consumed since last
 * call to this function.
 */
inline size_t
server_get_ticks_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->ticks), 0) - s->prevticks;
	s->prevticks += d;
	return d;
}

/**
 * Returns the wall-clock time in microseconds (us) consumed last send.
 */
inline size_t
server_get_waits(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->waits), 0);
}

/**
 * Returns the wall-clock time in microseconds (us) consumed since last
 * call to this function.
 */
inline size_t
server_get_waits_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->waits), 0) - s->prevwaits;
	s->prevwaits += d;
	return d;
}

/**
 * Returns the number of metrics sent since start.
 */
inline size_t
server_get_metrics(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->metrics), 0);
}

/**
 * Returns the number of metrics sent since last call to this function.
 */
inline size_t
server_get_metrics_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->metrics), 0) - s->prevmetrics;
	s->prevmetrics += d;
	return d;
}

/**
 * Returns the number of metrics dropped since start.
 */
inline size_t
server_get_dropped(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->dropped), 0);
}

/**
 * Returns the number of metrics dropped since last call to this function.
 */
inline size_t
server_get_dropped_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->dropped), 0) - s->prevdropped;
	s->prevdropped += d;
	return d;
}

/**
 * Returns the number of metrics requeued since start.
 */
inline size_t
server_get_requeue(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->requeue), 0);
}

/**
 * Returns the number of metrics requeued since last call to this function.
 */
inline size_t
server_get_requeue_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->requeue), 0) - s->prevrequeue;
	s->prevrequeue += d;
	return d;
}

/**
 * Returns the number of stalls since start.  A stall happens when the
 * queue is full, but it appears as if it would be a good idea to wait
 * for a brief period and retry.
 */
inline size_t
server_get_stalls(server *s)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->stalls), 0);
}

/**
 * Returns the number of stalls since last call to this function.
 */
inline size_t
server_get_stalls_sub(server *s)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->stalls), 0) - s->prevstalls;
	s->prevstalls += d;
	return d;
}

/**
 * Returns the (approximate) number of metrics waiting to be sent.
 */
inline size_t
server_get_queue_len(server *s)
{
	if (s == NULL)
		return 0;
	return queue_len(s->queue);
}

/**
 * Returns the allocated size of the queue backing metrics waiting to be
 * sent.
 */
inline size_t
server_get_queue_size(server *s)
{
	if (s == NULL)
		return 0;
	return queue_size(s->queue);
}

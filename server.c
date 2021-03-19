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
#include "queue.h"
#include "dispatcher.h"
#include "collector.h"
#include "server.h"
#include "cluster.h"

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

#define FAIL_WAIT_COUNT   6  /* 6 * 250ms = 1.5s */
#define DISCONNECT_WAIT_TIME   12  /* 12 * 250ms = 3s */
#define QUEUE_FREE_CRITICAL(FREE, s)  (FREE < __sync_fetch_and_add(&(s->qfree_threshold), 0))

#define SERVER_KEEP_RUNNING 1
#define SERVER_STOP 0 /* send all items with timeout from queue before shutdown */
#define SERVER_STOP_FORCE -1 /* don't send, may be for move items with swap shared queue */

int queuefree_threshold_start = 0;
int queuefree_threshold_end = 0;
long long shutdown_timeout = 120; /* 120s */

int server_timeout = 10 * 1000; /* 10s */

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

struct _servercon {
		pthread_t tid;
        int fd;
		unsigned short n;
		server *s;
        z_strm *strm;
        const char **batch; /* dequeued metrics */
        //struct timeval last;
        //struct timeval last_wait;
        char failure;       /* full byte for atomic access */
		char alive;       /* full byte for atomic access */
		char running;       /* full byte for atomic access */
        /* metrics */
        size_t metrics;
        size_t ticks;
        size_t prevmetrics;
        size_t prevticks;
};

struct _server {
	const char *ip;
	unsigned short port;
	char *instance;
	struct addrinfo *saddr;
	struct addrinfo *hint;
	unsigned short nconns; /* connections per server */
    struct _servercon conns[SERVER_MAX_CONNECTIONS];
	char reresolve:1;
	char shared_queue:2;
	queue *queue;
	size_t bsize;
	size_t qfree_threshold;
	volatile size_t threshold_start; /* qeueue threshold start */
    volatile size_t threshold_end;   /* qeueue threshold end */
	volatile size_t ttl;
	short iotimeout;
	unsigned int sockbufsize;
	unsigned char maxstalls:SERVER_STALL_BITS;
	con_type type;
	con_trnsp transport;
	con_proto ctype;
	struct _server **secondaries;
	size_t *secpos;
	size_t secondariescnt;
	char failover;
	char keep_running;  /* full byte for atomic access */
	unsigned char stallseq;  /* full byte for atomic access */
	size_t dropped;
	size_t stalls;
	size_t requeue;
	size_t prevdropped;
	size_t prevstalls;
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
	p = strm->obuf;
	len = strm->obuflen;
	/* Flush stream, this may not succeed completely due
	 * to flow control and whatnot, which the docs suggest need
	 * resuming to complete.  So, use a loop, but to avoid
	 * getting endlessly stuck on this, only try a limited
	 * number of times. */
	for (cnt = 0; cnt < SERVER_MAX_SEND; cnt++) {
		if ((slen = write(strm->hdl.sock, p, len)) != len) {
			if (slen >= 0) {
				struct pollfd ufds[1];
				p += slen;
				len -= slen;
				ufds[0].fd = strm->hdl.sock;
				ufds[0].events = POLLOUT;
				poll(ufds, 1, 1000);
			} else if (errno != EINTR) {
				return -1;
			}
		} else {
			strm->obuflen = 0;
			return 0;
		}
	}

	return -1;
}

static inline int
sockclose(z_strm *strm)
{
	return close(strm->hdl.sock);
}

static inline const char *
sockerror(z_strm *strm, int rval)
{
	(void)strm;
	(void)rval;
	return strerror(errno);
}

#ifdef HAVE_GZIP
/* gzip wrapped socket */
static inline int gzipflush(z_strm *strm);

static inline ssize_t
gzipwrite(z_strm *strm, const void *buf, size_t sze)
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
gzipflush(z_strm *strm)
{
	int cret;
	char cbuf[METRIC_BUFSIZ];
	size_t cbuflen;
	char *cbufp;
	int oret;

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
	free(strm->hdl.gz);
	return ret;
}

static inline const char *
gziperror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
}
#endif

#ifdef HAVE_LZ4
/* lz4 wrapped socket */

static inline int lzflush(z_strm *strm);

static inline ssize_t
lzwrite(z_strm *strm, const void *buf, size_t sze)
{
	/* ensure we have space available */
	if (strm->obuflen + sze > METRIC_BUFSIZ) {
		errno = ENOBUFS;
		return -1;
	}

	memcpy(strm->obuf + strm->obuflen, buf, sze);
	strm->obuflen += sze;

	return sze;
}

static inline int
lzflush(z_strm *strm)
{
	int oret;
	size_t ret;

	/* anything to do? */

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
	free(strm->hdl.z.cbuf);
	return strm->nextstrm->strmclose(strm->nextstrm);
}

static inline const char *
lzerror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
}
#endif

#ifdef HAVE_SNAPPY
/* snappy wrapped socket */
static inline int snappyflush(z_strm *strm);

static inline ssize_t
snappywrite(z_strm *strm, const void *buf, size_t sze)
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
snappyflush(z_strm *strm)
{
	int cret;
	char cbuf[METRIC_BUFSIZ];
	size_t cbuflen = sizeof(cbuf);
	char *cbufp = cbuf;
	int oret;

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

static inline const char *
snappyerror(z_strm *strm, int rval)
{
	return strm->nextstrm->strmerror(strm->nextstrm, rval);
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
	p = strm->obuf;
	len = strm->obuflen;
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
				ssl_err = SSL_get_error(strm->hdl.ssl, slen);
				if (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE)
					continue;
				else
					return slen;
			}
		} else {
			strm->obuflen = 0;
			return 0;
		}
	}

	return -1;
}

static inline int
sslclose(z_strm *strm)
{
	int sock = SSL_get_fd(strm->hdl.ssl);
	SSL_free(strm->hdl.ssl);
	return close(sock);
}

static char _sslerror_buf[256];
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
#endif

queue *server_set_shared_queue(server *s, queue *q)
{
        if (s->shared_queue && s->queue != q) {
                queue *qold = s->queue;
                s->queue = q;
                return qold;
        }
        return NULL;
}

char server_shared(server *s) {
    return s->shared_queue;
}

static inline char server_add_failure(server *s, unsigned short n) {
	char failure = __sync_fetch_and_add(&(s->conns[n].failure), 0);
	if (failure < FAIL_WAIT_COUNT) {
		/* prevent char overflow */
		__sync_fetch_and_add(&(s->conns[n].failure), 1);
	}

	return failure;
}

static int
server_disconnect(server *self, unsigned short n) {
	if (self->conns[n].fd != -1) {
		int ret = self->conns[n].strm->strmclose(self->conns[n].strm);
		self->conns[n].fd = -1;
		return ret;
	} else {
		return 0;
	}
}

char server_connect(server *self, unsigned short n)
{
	struct timeval timeout;
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
			if (server_add_failure(self, n) == 0)
				logerr("failed to resolve %s:%u (%u), server unavailable\n",
						self->ip, self->port, n);
			self->saddr = NULL;
			/* this will break out below */
		}
	}

	if (self->ctype == CON_PIPE) {
		int intconn[2];
		connection *conn;
		if (pipe(intconn) < 0) {
			if (server_add_failure(self, n) == 0)
				logerr("failed to create pipe: %s\n", strerror(errno));
			return -1;
		}
		conn = dispatch_addconnection(intconn[0], NULL, dispatch_worker_with_low_connections(), 0, 1);
		if (conn == NULL) {
			if (server_add_failure(self, n) == 0)
				logerr("failed to add pipe: %s\n", strerror(errno));
			return -1;
		}
		self->conns[n].fd = intconn[1];
	} else if (self->ctype == CON_FILE) {
		if ((self->conns[n].fd = open(self->ip,
						O_WRONLY | O_APPEND | O_CREAT,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		{
			if (server_add_failure(self, n) == 0)
				logerr("failed to open file '%s': %s\n",
						self->ip, strerror(errno));
			return -1;
		}
	} else if (self->ctype == CON_UDP) {
		struct addrinfo *walk;

		for (walk = self->saddr; walk != NULL; walk = walk->ai_next) {
			if ((self->conns[n].fd = socket(walk->ai_family,
							walk->ai_socktype,
							walk->ai_protocol)) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self, n) == 0)
					logerr("failed to create udp socket to %s:%u (%u): %s\n",
							self->ip, self->port, n, strerror(errno));
				return -1;
			}
			if (connect(self->conns[n].fd, walk->ai_addr, walk->ai_addrlen) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self, n) == 0)
					logerr("failed to connect udp socket to %s:%u (%u): %s\n",
							self->ip, self->port, n, strerror(errno));
				close(self->conns[n].fd);
				self->conns[n].fd = -1;
				return -1;
			}

			/* we made it up here, so this connection is usable */
			break;
		}
		/* if this didn't resolve to anything, treat as failure */
		if (self->saddr == NULL)
			server_add_failure(self, n);
		/* if all addrinfos failed, try again later */
		if (self->conns[n].fd < 0)
			return -1;
	} else {  /* CON_TCP */
		int ret;
		int args;
		struct addrinfo *walk;

		for (walk = self->saddr; walk != NULL; walk = walk->ai_next) {
			if ((self->conns[n].fd = socket(walk->ai_family,
							walk->ai_socktype,
							walk->ai_protocol)) < 0)
			{
				if (walk->ai_next == NULL &&
						server_add_failure(self, n) == 0)
					logerr("failed to create socket to %s:%u (%u): %s\n",
							self->ip, self->port, n, strerror(errno));
				return -1;
			}

			/* put socket in non-blocking mode such that we can
			 * poll() (time-out) on the connect() call */
			args = fcntl(self->conns[n].fd, F_GETFL, NULL);
			if (fcntl(self->conns[n].fd, F_SETFL, args | O_NONBLOCK) < 0) {
				logerr("failed to set socket non-blocking mode to %s:%u (%u): %s\n",
						self->ip, self->port, n, strerror(errno));
				server_add_failure(self, n);
				close(self->conns[n].fd);
				self->conns[n].fd = -1;
				return -1;
			}
			ret = connect(self->conns[n].fd, walk->ai_addr, walk->ai_addrlen);

			if (ret < 0 && errno == EINPROGRESS) {
				/* wait for connection to succeed if the OS thinks
				 * it can succeed */
				struct pollfd ufds[1];
				ufds[0].fd = self->conns[n].fd;
				ufds[0].events = POLLIN | POLLOUT;
				ret = poll(ufds, 1, self->iotimeout + (rand2() % 100));
				if (ret == 0) {
					/* time limit expired */
					if (walk->ai_next == NULL &&
							server_add_failure(self, n) == 0)
						logerr("failed to connect() to "
								"%s:%u (%u): Operation timed out\n",
								self->ip, self->port, n);
					close(self->conns[n].fd);
					self->conns[n].fd = -1;
					return -1;
				} else if (ret < 0) {
					/* some select error occurred */
					if (walk->ai_next &&
							server_add_failure(self, n) == 0)
						logerr("failed to poll() for %s:%u (%u): %s\n",
								self->ip, self->port, n, strerror(errno));
					close(self->conns[n].fd);
					self->conns[n].fd = -1;
					return -1;
				} else {
					if (ufds[0].revents & POLLHUP) {
						if (walk->ai_next == NULL &&
								server_add_failure(self, n) == 0)
							logerr("failed to connect() for %s:%u (%u): "
									"Connection refused\n",
									self->ip, self->port, n);
						close(self->conns[n].fd);
						self->conns[n].fd = -1;
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
						server_add_failure(self, n) == 0)
				{
					logerr("failed to connect() to %s:%u (%u): %s\n",
							self->ip, self->port, n, strerror(errno));
					dispatch_check_rlimit_and_warn();
				}
				close(self->conns[n].fd);
				self->conns[n].fd = -1;
				return -1;
			}

			/* make socket blocking again */
			if (fcntl(self->conns[n].fd, F_SETFL, args) < 0) {
				logerr("failed to remove socket non-blocking "
						"mode to %s:%u (%u): %s\n", 
						self->ip, self->port, n, strerror(errno));
				server_add_failure(self, n);
				close(self->conns[n].fd);
				self->conns[n].fd = -1;
				return -1;
			}

			/* disable Nagle's algorithm, issue #208 */
			args = 1;
			if (setsockopt(self->conns[n].fd, IPPROTO_TCP, TCP_NODELAY,
						&args, sizeof(args)) != 0)
				; /* ignore */

#ifdef TCP_USER_TIMEOUT
			/* break out of connections when no ACK is being
			 * received for +- 10 seconds instead of
			 * retransmitting for +- 15 minutes available on
			 * linux >= 2.6.37
			 * the 10 seconds is in line with the SO_SNDTIMEO
			 * set on the socket below */
			args = 10000 + (rand2() % 300);
			if (setsockopt(self->conns[n].fd, IPPROTO_TCP, TCP_USER_TIMEOUT,
						&args, sizeof(args)) != 0)
				; /* ignore */
#endif

			/* if we reached up here, we're good to go, so don't
			 * continue with the other addrinfos */
			break;
		}
		/* if this didn't resolve to anything, treat as failure */
		if (self->saddr == NULL)
			server_add_failure(self, n);
		/* all available addrinfos failed on us */
		if (self->conns[n].fd < 0)
			return -1;
	}

	/* ensure we will break out of connections being stuck more
	 * quickly than the kernel would give up */
	timeout.tv_sec = 10;
	timeout.tv_usec = (rand2() % 300) * 1000;
	if (setsockopt(self->conns[n].fd, SOL_SOCKET, SO_SNDTIMEO,
				&timeout, sizeof(timeout)) != 0)
		; /* ignore */
	if (self->sockbufsize > 0)
		if (setsockopt(self->conns[n].fd, SOL_SOCKET, SO_SNDBUF,
					&self->sockbufsize, sizeof(self->sockbufsize)) != 0)
			; /* ignore */

	self->conns[n].strm->obuflen = 0;
#ifdef SO_NOSIGPIPE
	if (self->ctype == CON_TCP || self->ctype == CON_UDP) {
		int enable = 1;
		if (setsockopt(self->conns[n].fd, SOL_SOCKET, SO_NOSIGPIPE,
					(void *)&enable, sizeof(enable)) != 0)
			logout("warning: failed to ignore SIGPIPE on socket to %s:%u (%u): %s\n",
					self->ip, self->port, n, strerror(errno));
	}
#endif

#ifdef HAVE_GZIP
	if ((self->transport & 0xFFFF) == W_GZIP) {
		self->conns[n].strm->hdl.gz = malloc(sizeof(z_stream));
		if (self->conns[n].strm->hdl.gz != NULL) {
			self->conns[n].strm->hdl.gz->zalloc = Z_NULL;
			self->conns[n].strm->hdl.gz->zfree = Z_NULL;
			self->conns[n].strm->hdl.gz->opaque = Z_NULL;
			self->conns[n].strm->hdl.gz->next_in = Z_NULL;
		}
		if (self->conns[n].strm->hdl.gz == NULL ||
				deflateInit2(self->conns[n].strm->hdl.gz,
					Z_DEFAULT_COMPRESSION,
					Z_DEFLATED,
					15 + 16,
					8,
					Z_DEFAULT_STRATEGY) != Z_OK)
		{
			server_add_failure(self, n);
			logerr("failed to open gzip stream: out of memory\n");
			close(self->conns[n].fd);
			self->conns[n].fd = -1;
			return -1;
		}
	}
#endif
#ifdef HAVE_LZ4
	if ((self->transport & 0xFFFF) == W_LZ4) {
		self->conns[n].strm->obuflen = 0;

		/* get the maximum size that should ever be required and allocate for it */

		self->conns[n].strm->hdl.z.cbuflen = LZ4F_compressFrameBound(sizeof(self->conns[n].strm->obuf), NULL);
		if ((self->conns[n].strm->hdl.z.cbuf = malloc(self->conns[n].strm->hdl.z.cbuflen)) == NULL) {
			server_add_failure(self, n);
			logerr("Failed to allocate %lu bytes for compressed LZ4 data\n", self->conns[n].strm->hdl.z.cbuflen);
			close(self->conns[n].fd);
			self->conns[n].fd = -1;
			return -1;
		}
	}
#endif
#ifdef HAVE_SSL
	if ((self->transport & ~0xFFFF) == W_SSL) {
		int rv;
		z_strm *sstrm;

		if ((self->transport & 0xFFFF) == W_PLAIN) { /* just SSL, nothing else */
			sstrm = self->conns[n].strm;
		} else {
			sstrm = self->conns[n].strm->nextstrm;
		}

		sstrm->hdl.ssl = SSL_new(sstrm->ctx);
		SSL_set_tlsext_host_name(sstrm->hdl.ssl, self->ip);
		if (SSL_set_fd(sstrm->hdl.ssl, self->conns[n].fd) == 0) {
			server_add_failure(self, n);
			logerr("failed to SSL_set_fd: %s\n",
					ERR_reason_error_string(ERR_get_error()));
			sstrm->strmclose(sstrm);
			self->conns[n].fd = -1;
			return -1;
		}
		if ((rv = SSL_connect(sstrm->hdl.ssl)) != 1) {
			server_add_failure(self, n);
			logerr("failed to connect ssl stream: %s\n",
					sslerror(sstrm, rv));
			sstrm->strmclose(sstrm);
			self->conns[n].fd = -1;
			return -1;
		}
		if ((rv = SSL_get_verify_result(sstrm->hdl.ssl)) != X509_V_OK) {
			server_add_failure(self, n);
			logerr("failed to verify ssl certificate: %s\n",
					sslerror(sstrm, rv));
			sstrm->strmclose(sstrm);
			self->conns[n].fd = -1;
			return -1;
		}
	} else
#endif
	{
		z_strm *pstrm;

		if (self->transport == W_PLAIN) { /* just plain socket */
			pstrm = self->conns[n].strm;
		} else {
			pstrm = self->conns[n].strm->nextstrm;
		}
		pstrm->hdl.sock = self->conns[n].fd;
	}
	return self->conns[n].fd;
}

queue *server_queue(server *s)
{
	return s->queue;
}

static inline int server_secpos_alloc(server *self)
{
	int i;
	self->secpos = malloc(sizeof(size_t) * self->secondariescnt);
	if (self->secpos == NULL) {
		logerr("server: failed to allocate memory "
				"for secpos\n");
		return -1;
	}
	for (i = 0; i < self->secondariescnt; i++)
		self->secpos[i] = i;
	return 0;
}

static int
server_poll(server *self, unsigned short n, int timeout_ms)
{
	if (self->conns[n].fd > 0) {
		int ret;
		struct pollfd ufds[1];
		ufds[0].fd = self->conns[n].fd;
		ufds[0].events = POLLOUT;
		ret = poll(ufds, 1, timeout_ms);
		if (ret < 0) {
			logerr("poll error to %s:%u (%u): %s", self->ip, self->port, n, strerror(errno));
			server_disconnect(self, n);
		} else if (ret > 0) {
			if (ufds[0].revents & POLLOUT) {
				return 0;
			} else if (ufds[0].revents & POLLHUP) {
				if (server_add_failure(self, n) == 0)
					logerr("closed connection to %s:%u (%u)\n",
							self->ip, self->port, n);
			} else {
				if (server_add_failure(self, n) == 0)
					logerr("poll error event %d for connection to %s:%u (%u)\n",
						ufds[0].revents, self->ip, self->port, n);
			}
		} else {
			/* timeout */
			logerr("timeout write to %s:%u (%u): %s", self->ip, self->port, n);
			server_disconnect(self, n);
		}
	} else {
		errno = ENOTCONN;
	}
	return -1;
}

static ssize_t server_queuesend(server *self, queue *squeue, server *source, unsigned short n)
{
	const char **metric = self->conns[n].batch;
	const char **p;
	size_t len;
	ssize_t slen;

	if (server_poll(self, n, server_timeout) == -1) {
		return -1;
	}

	*metric = NULL;

	len = queue_dequeue_vector(self->conns[n].batch, squeue, self->bsize);

	if (len != 0) {
		size_t sended = 0, total = 0, flushed = 0;
		__sync_bool_compare_and_swap(&(self->conns[n].alive), 0, 1);
		self->conns[n].batch[len] = NULL;
		metric = self->conns[n].batch;	
		for (; *metric != NULL; metric++) {
			size_t mlen = *(size_t *)(*metric);
			const char *m = *metric + sizeof(size_t);
			slen = self->conns[n].strm->strmwrite(self->conns[n].strm, m, mlen);
			if (slen == -1 && errno == ENOBUFS) {
				/* Flush stream */
				if (self->conns[n].strm->strmflush(self->conns[n].strm) >= 0) {
					/* retry write after flush */
					__sync_add_and_fetch(&(self->conns[n].metrics), sended);
					flushed += sended;
					sended = 0;
					if (server_poll(self, n, server_timeout) == -1) {
						break;
					}
					if (slen == -1) {
						slen = self->conns[n].strm->strmwrite(self->conns[n].strm, m, mlen);
					}
				} else {
					slen = -1;
					break;
				}
			}
			
			if (slen == mlen) {
				sended++;
				total++;
			} else {
				/* not fully sent (after tries), or failure
				 * close connection regardless so we don't get
				 * synchonisation problems */
				if (server_add_failure(self, n) == 0)
					logerr("failed to write() to %s:%u (%u): %s\n",
							self->ip, self->port, n,
							(slen < 0 ?
							 self->conns[n].strm->strmerror(self->conns[n].strm, slen) :
							 "incomplete write"));
				server_disconnect(self, n);
				break;
			}
		}

		if (flushed < total) {
			if (server_poll(self, n, server_timeout) == 0) {
				if (self->conns[n].strm->strmflush(self->conns[n].strm) < 0) {
					if (server_add_failure(self, n) == 0)
						logerr("failed to write() to %s:%u (%u): %s\n",
								self->ip, self->port, n,
								(slen < 0 ?
								self->conns[n].strm->strmerror(self->conns[n].strm, slen) :
								"incomplete write"));
					server_disconnect(self, n);
				}
			}
		}
		
		if (self->conns[n].fd >= 0) {
			if ( __sync_add_and_fetch(&(self->conns[n].failure), 0) > 0) {
				if (__sync_sub_and_fetch(&(self->conns[n].failure), 1) == 0 && self->ctype != CON_UDP) {
					logerr("server %s:%u (%u): OK\n", self->ip, self->port, n);
				}
			}
			for (p = self->conns[n].batch; *p != NULL; p++) {
				free((char *)*p);
			}
		} else {
			/* reset metric location for requeue metrics from possible unsended buffer
			* can duplicate already sended message
			*/
			metric = self->conns[n].batch + flushed;
			self->conns[n].strm->obuflen = 0;
			/* free sended metrics */
			for (p = self->conns[n].batch; p != metric && *p != NULL; p++) {
				free((char *)*p);
			}
			/* put back stuff we couldn't process */
			for (; *metric != NULL; metric++) {
				if (!queue_putback(squeue, *metric)) {
					if (mode & MODE_DEBUG)
						logerr("server %s:%u: dropping metric: %s",
								source->ip, source->port,
								*metric + sizeof(size_t));
					free((char *)*metric);
					__sync_add_and_fetch(&(source->dropped), 1);
				}
			}
		}
	}

	if (self->conns[n].fd == -1)
		return -1;
	else
		return (ssize_t) len;
}

char server_failed(server *self) {
	int i;
	char failure, min_failure = 0;
	for (i = 0; i < self->nconns; i++) {
		failure = __sync_add_and_fetch(&(self->conns[i].failure), 0);
		if (failure == 0) {
			return 0;
		} else if (min_failure == 0 || min_failure < failure) {
			min_failure = failure;
		}
	}

	return min_failure;
}

char server_alive(server *self) {
	int i;
	for (i = 0; i < self->nconns; i++) {
		if (__sync_add_and_fetch(&(self->conns[i].alive), 0) == 0) {
			return 0;
		}
	}

	return 1;
}

static ssize_t server_queueread(server *self, int shutdown, char *idle, unsigned short n)
{
	ssize_t len = 0, ret = 0;
	size_t qfree;
	int i, need_backup = 0;
	struct timeval start, stop;

	qfree = queue_free(self->queue);
	if (self->secondariescnt > 0) {
		if (QUEUE_FREE_CRITICAL(qfree, self)) {
			need_backup = 1;
			/* destination overloaded, set threshold for destination recovery */
			if (__sync_bool_compare_and_swap(&(self->qfree_threshold), self->threshold_start, self->threshold_end))
				logerr("throttle %s:%u: waiting for %zu metrics\n",
					self->ip, self->port, queue_len(self->queue));
		} else {
			/* threshold for cancel rebalance */
			if (__sync_bool_compare_and_swap(&(self->qfree_threshold), self->threshold_end, self->threshold_start))
				logerr("throttle %s:%u: end waiting for %zu metrics\n",
						self->ip, self->port, queue_len(self->queue));
		}

		if (self->failover) {
			if (need_backup == 0) {
				/* check for failover needed */
				for (i = 0; i < self->secondariescnt; i++) {
					if (self == self->secondaries[i]) {
						need_backup = 1;
						break;
					}
					else if (server_failed(self->secondaries[i]) < FAIL_WAIT_COUNT) {
						if (self->ctype == CON_TCP && self->conns[n].fd >= 0 &&
								*idle++ > DISCONNECT_WAIT_TIME)
						{
							server_disconnect(self, n);
						}
						return 0;
					}
				}
			}
		}
	}

	if (qfree == queue_size(self->queue)) {
		/* if we're idling, close the TCP connection, this allows us
		 * to reduce connections, while keeping the connection alive
		 * if we're writing a lot */
		if (self->ctype == CON_TCP && self->conns[n].fd >= 0 &&
				*idle++ > DISCONNECT_WAIT_TIME)
		{
			server_disconnect(self, n);
		}
		/* if we are in failure mode, keep checking if we can
		 * connect, this avoids unnecessary queue moves */
		if (self->secondariescnt == 0)
			return 0;
		else if (shutdown != SERVER_KEEP_RUNNING) {
			int noqueue = 0;
			for (i = 0; i < self->secondariescnt; i++) {
				if (self == self->secondaries[i])
					continue;
				else if (queue_free(self->secondaries[i]->queue) < queue_len(self->secondaries[i]->queue)) {
					noqueue = 1;
					break;
				}
			}
			if (noqueue == 0)
				return 0;
		}
	}

	/* at this point we've got work to do, if we're instructed to
	 * shut down, however, try to get everything out of the door
	 * (until we fail, see top of this loop) */
	/* try to connect */
	gettimeofday(&start, NULL);
	if (self->conns[n].fd < 0 && server_connect(self, n) == -1) {
		gettimeofday(&stop, NULL);
		__sync_add_and_fetch(&(self->conns[n].ticks), timediff(start, stop));
		return -1;
	}

	/* send up to batch size */
	if (qfree < queue_size(self->queue)) {
		if (shutdown != SERVER_KEEP_RUNNING) {
			/* be noisy during shutdown so we can track any slowing down
			 * servers, possibly preventing us to shut down */
			logerr("shutting down %s:%u: waiting for %zu metrics\n",
					self->ip, self->port, queue_len(self->queue));
		}
		len = server_queuesend(self, self->queue, self, n);
		if (len < 0)
			return -1;
	}

	if (!self->shared_queue && self->secondariescnt > 0) {
		size_t sqfree;
		for (i = 0; i < self->secondariescnt; i++) {
			if (self == self->secondaries[i])
		 		continue;
			sqfree = queue_free(self->secondaries[i]->queue);
			if (sqfree < qfree && 
					(QUEUE_FREE_CRITICAL(sqfree, self->secondaries[i]) || server_failed(self->secondaries[i]))) {
				/* read from busy secondaries */
				ret = server_queuesend(self, self->secondaries[i]->queue, self->secondaries[i], n);
				if (ret < 0)
					break;
				__sync_add_and_fetch(&(self->secondaries[i]->requeue), ret);
				len += ret;
			}
		}
	}

	gettimeofday(&stop, NULL);
	__sync_add_and_fetch(&(self->conns[n].ticks), timediff(start, stop));

	*idle = 0;

	if (ret >= 0) {
		return len;
	} else
		return -1;
}

/**
 * Reads from the queue and sends items to the remote server.  This
 * function is designed to be a thread.  Data sending is attempted to be
 * batched, but sent one by one to reduce loss on sending failure.
 * A connection with the server is maintained for as long as there is
 * data to be written.  As soon as there is none, the connection is
 * dropped if a timeout of DISCONNECT_WAIT_TIME exceeds.
 */
static void *
server_queuereader(void *d)
{
	struct _servercon *sc = (struct _servercon *)d;
	server *self = sc->s;
	char idle = 0;
	int	shutdown_init = SERVER_KEEP_RUNNING;
	time_t started, ended;

	ssize_t ret;

	__sync_bool_compare_and_swap(&(self->conns[sc->n].running), 0, 1);
	__sync_bool_compare_and_swap(&(self->conns[sc->n].alive), 0, 1);

	started = time(NULL);

	while (1) {
		int shutdown = __sync_add_and_fetch(&(self->keep_running), 0);
		if (shutdown == SERVER_STOP_FORCE)
				break;		
		if (shutdown_init != shutdown) {
			shutdown_init = shutdown;
			started = time(NULL);
		}

		ret = server_queueread(self, shutdown, &idle, sc->n);
		if (ret == 0)
			__sync_bool_compare_and_swap(&(self->conns[sc->n].alive), 1, 0);
		else
			__sync_bool_compare_and_swap(&(self->conns[sc->n].alive), 0, 1);

		if (shutdown == SERVER_STOP) {
			ended = time(NULL);
			if (ended - started >= shutdown_timeout) {
				break;
			}
			if (self->secondariescnt > 0) {
				int c;
				char empty = 1;
				/* workaraound: prevent exit last live reader
				* if other server_queuereader read last metrics from queue and can't send it
				*/
				for (c = 0; c < self->secondariescnt; c++) {
					if (self != self->secondaries[c] &&
						(queue_free(self->secondaries[c]->queue) < queue_len(self->secondaries[c]->queue) ||
						server_alive(self->secondaries[c]))) {
						empty = 0;
						break;
					}
				}
				if (empty) {
					break;
				}
			} else {
				break;
			}
		} else	if (self->ttl > 0) {
			ended = time(NULL);
			if (ended - started >= self->ttl) {
				server_disconnect(self, sc->n);
				started = ended;
				logout("connection ttl reached for %s:%u (%u)\n",
					self->ip, self->port, sc->n);
			}
		}

		if (ret < self->bsize) {
			usleep((100 + (rand2() % 100)) * 1000);  /* 100ms - 200ms */
		}
	}
	__sync_bool_compare_and_swap(&(self->conns[sc->n].alive), 1, 0);
	logout("shut down %s:%d (%u)\n", self->ip, self->port, sc->n);
	if (self->conns[sc->n].fd >= 0) {
		self->conns[sc->n].strm->strmflush(self->conns[sc->n].strm);
		server_disconnect(self, sc->n);
	}

	__sync_bool_compare_and_swap(&(self->conns[sc->n].running), 1, 0);
	return NULL;
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
                case LB:
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

int
cluster_set_threshold(cluster *cl, int threshold_start, int threshold_end)
{
	if (threshold_start < 0 || threshold_start > 100 ||
		threshold_end < 0 || threshold_end > 100) {
			logerr("queue threshold needs to be a number >=0 and <=100 for cluster '%s'\n", cl->name);
			return -1;
	}
	if (threshold_end < threshold_start) {
		logerr("queue threshold end needs to be greater than start for cluster '%s'\n", cl->name);
		return -1;
	}

	cl->threshold_start = threshold_start;
	cl->threshold_end = threshold_end;

	return 0;
}

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
		unsigned short nconns,
		struct addrinfo *saddr,
		struct addrinfo *hint,
		size_t qsize,
		queue *q,
		size_t bsize,
		int maxstalls,
		unsigned short iotimeout,
		unsigned int sockbufsize,
		int threshold_start,
        int threshold_end,
		size_t ttl)
{
	server *ret;
	unsigned short i;

	if ((ret = malloc(sizeof(server))) == NULL)
		return NULL;

	ret->ttl = ttl;
	ret->secpos = NULL;
	ret->type = type;
	ret->transport = transport;
	ret->ctype = ctype;
	ret->secpos = NULL;
	ret->secondaries = NULL;
	ret->secondariescnt = 0;
	if (nconns > 1 && (ctype == CON_TCP || ctype == CON_UDP)) {
		if (nconns > SERVER_MAX_CONNECTIONS)
				ret->nconns = SERVER_MAX_CONNECTIONS;
		else
				ret->nconns = nconns;
	} else {
			ret->nconns = 1; /* for other type force 1 connection */
	}
	if (threshold_start < 1)  {
		ret->threshold_start = queuefree_threshold_start;
	} else {
		ret->threshold_start = threshold_start;
	}
    if (threshold_end < 1)  {
		ret->threshold_end = queuefree_threshold_end;
	} else {
		ret->threshold_end = threshold_end;
	}

	for (i = 0; i < SERVER_MAX_CONNECTIONS; i++) {
		ret->conns[i].fd = -1;
		ret->conns[i].tid = 0;
		ret->conns[i].n = i;
		ret->conns[i].strm = NULL;
		ret->conns[i].batch = NULL;
		ret->conns[i].s = ret;

		// ret->conns[i].last.tv_sec = 0;
		// ret->conns[i].last.tv_usec = 0;
		// ret->conns[i].last_wait.tv_sec = 0;
		// ret->conns[i].last_wait.tv_usec = 0;

		ret->conns[i].failure = 0;
		ret->conns[i].alive = 0;
		ret->conns[i].running = 0;

		ret->conns[i].metrics = 0;      
		ret->conns[i].ticks = 0;
		ret->conns[i].prevmetrics = 0;
		ret->conns[i].prevticks = 0;
	}

	ret->ip = NULL;
	ret->saddr = saddr;
	ret->reresolve = 0;
	ret->hint = NULL;
	if (hint != NULL) {
		ret->reresolve = 1;
		ret->hint = hint;
	}	

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

	ret->ip = strdup(ip);
	if (ret->ip == NULL) {
		server_cleanup(ret);
		return NULL;
	}

	ret->port = port;
	ret->instance = NULL;
	ret->bsize = bsize;
	ret->qfree_threshold = queuefree_threshold_start;
	ret->iotimeout = iotimeout < 250 ? 600 : iotimeout;
	ret->sockbufsize = sockbufsize;
	ret->maxstalls = maxstalls;

	for (i = 0; i < ret->nconns; i++) {
		if ((ret->conns[i].batch = malloc(sizeof(char *) * (bsize + 1))) == NULL) {
			server_cleanup(ret);
			return NULL;
		}
		if ((ret->conns[i].strm = malloc(sizeof(z_strm))) == NULL) {
			server_cleanup(ret);
			return NULL;
		}
		ret->conns[i].strm->obuflen = 0;
		ret->conns[i].strm->nextstrm = NULL;

		/* setup normal or SSL-wrapped socket first */
	#ifdef HAVE_SSL
		if ((transport & ~0xFFFF) == W_SSL) {
			/* create an auto-negotiate context */
			const SSL_METHOD *m = SSLv23_client_method();
			ret->conns[i].strm->ctx = SSL_CTX_new(m);
		
			if (sslCA != NULL) {
				if (ret->conns[i].strm->ctx == NULL ||
					SSL_CTX_load_verify_locations(ret->conns[i].strm->ctx,
												sslCAisdir ? NULL : sslCA,
												sslCAisdir ? sslCA : NULL) == 0)
				{
					char *err = ERR_error_string(ERR_get_error(), NULL);
					logerr("failed to load SSL verify locations from %s for "
							"%s:%d: %s\n", sslCA, ret->ip, ret->port, err);
					server_cleanup(ret);
					return NULL;
				}
			}
			SSL_CTX_set_verify(ret->conns[i].strm->ctx, SSL_VERIFY_PEER, NULL);

			ret->conns[i].strm->strmwrite = &sslwrite;
			ret->conns[i].strm->strmflush = &sslflush;
			ret->conns[i].strm->strmclose = &sslclose;
			ret->conns[i].strm->strmerror = &sslerror;
		} else
	#endif
		{
			ret->conns[i].strm->strmwrite = &sockwrite;
			ret->conns[i].strm->strmflush = &sockflush;
			ret->conns[i].strm->strmclose = &sockclose;
			ret->conns[i].strm->strmerror = &sockerror;
		}
		
		/* now see if we have a compressor defined */
		if ((transport & 0xFFFF) == W_PLAIN) {
			/* catch noop */
		}
	#ifdef HAVE_GZIP
		else if ((transport & 0xFFFF) == W_GZIP) {
			z_strm *gzstrm = malloc(sizeof(z_strm));
			if (gzstrm == NULL) {
				server_cleanup(ret);
				return NULL;
			}
			gzstrm->strmwrite = &gzipwrite;
			gzstrm->strmflush = &gzipflush;
			gzstrm->strmclose = &gzipclose;
			gzstrm->strmerror = &gziperror;
			gzstrm->nextstrm = ret->conns[i].strm;
			gzstrm->obuflen = 0;
			ret->conns[i].strm = gzstrm;
		}
	#endif
	#ifdef HAVE_LZ4
		else if ((transport & 0xFFFF) == W_LZ4) {
			z_strm *lzstrm = malloc(sizeof(z_strm));
			if (lzstrm == NULL) {
				server_cleanup(ret);
				return NULL;
			}
			lzstrm->strmwrite = &lzwrite;
			lzstrm->strmflush = &lzflush;
			lzstrm->strmclose = &lzclose;
			lzstrm->strmerror = &lzerror;
			lzstrm->nextstrm = ret->conns[i].strm;
			lzstrm->obuflen = 0;
			ret->conns[i].strm = lzstrm;
		}
	#endif
	#ifdef HAVE_SNAPPY
		else if ((transport & 0xFFFF) == W_SNAPPY) {
			z_strm *snpstrm = malloc(sizeof(z_strm));
			if (snpstrm == NULL) {
				server_cleanup(ret);
				return NULL;
			}
			snpstrm->strmwrite = &snappywrite;
			snpstrm->strmflush = &snappyflush;
			snpstrm->strmclose = &snappyclose;
			snpstrm->strmerror = &snappyerror;
			snpstrm->nextstrm = ret->conns[i].strm;
			snpstrm->obuflen = 0;
			ret->conns[i].strm = snpstrm;
		}
	#endif

	}

	ret->failover = 0;
	ret->keep_running = 1;
	ret->stallseq = 0;

	ret->dropped = 0;
	ret->requeue = 0;
	ret->stalls = 0;
	ret->prevdropped = 0;
	ret->prevrequeue = 0;
	ret->prevstalls = 0;

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
server_cmp(server *s, struct addrinfo *saddr, const char *ip, unsigned short port, con_proto proto, unsigned short nconns)
{
	if (nconns != s->nconns) {
		return 1; /* force not equal */
	}
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

void
server_set_port(server *s, unsigned short port) {
	s->port = port;
}

/**
 * Starts a previously created server using server_new().  Returns
 * errno if starting a thread failed, after which the caller should
 * server_free() the given s pointer.
 */
int
server_start(server *s)
{
	unsigned short i;
	int ret;
	if (server_secpos_alloc(s) == -1)
		return -1;
	for (i = 0; i < s->nconns; i++) {
		ret = pthread_create(&s->conns[i].tid, NULL, &server_queuereader, &(s->conns[i]));
		if (ret != 0)
			return ret;
	}
	return 0;
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
	/* check for overloaded destination */
	if (!force && s->secondariescnt > 0 &&
		(server_failed(s) || QUEUE_FREE_CRITICAL(qfree, s))) {
		size_t i;
		size_t maxqfree = qfree + 4 * s->bsize;
		queue *squeue = s->queue;

		for (i = 0; i < s->secondariescnt; i++) {
			if (s != s->secondaries[i] && !server_failed(s) &&
					__sync_bool_compare_and_swap(&(s->secondaries[i]->keep_running), 1, 1)) {
				size_t sqfree = queue_free(s->secondaries[i]->queue);
				if (sqfree >  maxqfree) {
					squeue = s->secondaries[i]->queue;
					maxqfree = sqfree;
				}
			}
		}
		if (squeue != s->queue) {
			if (!server_failed(s))
				__sync_add_and_fetch(&(s->requeue), 1);
			queue_enqueue(squeue, d);
			return 1;
		}
	}
	if (qfree == 0) {
		char failure = server_failed(s);
		if (!force && s->secondariescnt > 0) {
			size_t i;
			/* don't immediately drop if we know there are others that
			 * back us up */
			for (i = 0; i < s->secondariescnt; i++) {
				if (!server_failed(s->secondaries[i])) {
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

void cluster_shutdown(cluster *c, int swap)
{
	servers *s;

	for (s = cluster_servers(c); s != NULL; s = s->next) {
		server_shutdown(s->server, swap);
	}
}

void cluster_shutdown_wait(cluster *c)
{
	servers *s;

	for (s = cluster_servers(c); s != NULL; s = s->next) {
		server_shutdown_wait(s->server);
	}
}

/**
 * Tells this server to finish sending pending items from its queue.
 */
void
server_shutdown(server *s, int swap)
{
	if (swap) {
		__sync_bool_compare_and_swap(&(s->keep_running), SERVER_KEEP_RUNNING, SERVER_STOP_FORCE);
	} else {
		__sync_bool_compare_and_swap(&(s->keep_running), SERVER_KEEP_RUNNING, SERVER_STOP);
	}
}

void server_shutdown_wait(server *s)
{
	int i, n;
	/* wait for the secondaries to be stopped so we surely don't get
     * invalid reads when server_free is called */
	if (s->secondariescnt > 0) {
		for (i = 0; i < s->secondariescnt; i++) {
			for (n = 0; n < s->secondaries[i]->nconns; n++) {
				while (!__sync_bool_compare_and_swap(&(s->secondaries[i]->conns[n].running), 0, 0))
					usleep(200 * 1000);
			}
		}
	} else {
		for (n = 0; n < s->nconns; n++) {
			while (!__sync_bool_compare_and_swap(&(s->conns[n].running), 0, 0))
		   		usleep(200 * 1000);
		}
	}
}

/**
 * Frees this server and associated resources.  This MOT includes joining
 * the server thread.
 */
void
server_cleanup(server *s) {
	int i;

	if (s->ctype == CON_TCP) {
		size_t qlen = queue_len(s->queue);
		if (qlen > 0)
			logerr("dropping %zu metrics for %s:%u\n",
					qlen, s->ip, s->port);
	}

	if (!s->shared_queue) {
		queue_destroy(s->queue);
	}
	for (i = 0; i < s->nconns; i++) {
		free(s->conns[i].batch);
		if (s->conns[i].strm->nextstrm != NULL)
			free(s->conns[i].strm->nextstrm);
		free(s->conns[i].strm);
	}
	if (s->instance)
		free(s->instance);
	if (s->saddr != NULL)
		freeaddrinfo(s->saddr);
	if (s->hint)
		freeaddrinfo(s->hint);
	free((char *)s->ip);	
	s->ip = NULL;
	free(s->secpos);
	free(s);
}

/**
 * Frees this server and associated resources.  This includes joining
 * the server thread.
 */
void
server_free(server *s) {
	int err;
	unsigned short i;

	for (i = 0; i < s->nconns; i++) {
		if (s->conns[i].tid != 0 && (err = pthread_join(s->conns[i].tid, NULL)) != 0)
			logerr("%s:%u (%u): failed to join server thread: %s\n",
					s->ip, s->port, s->conns[i].n, strerror(err));
		s->conns[i].tid = 0;
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
	char shared_queue;
	unsigned short i;
	queue *t;

	t = l->queue;
	l->queue = r->queue;
	r->queue = t;
	shared_queue = l->shared_queue;
	l->shared_queue = r->shared_queue;
	r->shared_queue = shared_queue;
	
	/* swap associated statistics as well */
	l->dropped = r->dropped;
	l->stalls = r->stalls;
	l->requeue = r->requeue;
	l->prevdropped = r->prevdropped;
	l->prevstalls = r->prevstalls;
	l->prevrequeue = r->prevrequeue;

	for (i = 0; i < l->nconns && i < r->nconns; i++) {
		l->conns[i].metrics = r->conns[i].metrics;
		l->conns[i].ticks = r->conns[i].ticks;
		l->conns[i].prevmetrics = r->conns[i].prevmetrics;		
		l->conns[i].prevticks = r->conns[i].prevticks;
	}
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
 * Returns the wall-clock time in microseconds (us) consumed sending metrics.
 */
inline size_t
server_get_ticks(server *s, unsigned short n)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->conns[n].ticks), 0);
}

/**
 * Returns the wall-clock time in microseconds (us) consumed since last
 * call to this function.
 */
inline size_t
server_get_ticks_sub(server *s, unsigned short n)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->conns[n].ticks), 0) - s->conns[n].prevticks;
	s->conns[n].prevticks += d;
	return d;
}

/**
 * Returns the fails count.
 */
inline size_t
server_get_failure(server *s, unsigned short n)
{
	if (s == NULL)
		return 0;
	else {
		char failure = __sync_add_and_fetch(&(s->conns[n].failure), 0);
		if (failure < 1)
			return 0;
		else
			return (size_t) failure;
	}
}

/**
 * Returns the number of metrics sent since start.
 */
inline size_t
server_get_metrics(server *s, unsigned short n)
{
	if (s == NULL)
		return 0;
	return __sync_add_and_fetch(&(s->conns[n].metrics), 0);
}

/**
 * Returns the number of metrics sent since last call to this function.
 */
inline size_t
server_get_metrics_sub(server *s, unsigned short n)
{
	size_t d;
	if (s == NULL)
		return 0;
	d = __sync_add_and_fetch(&(s->conns[n].metrics), 0) - s->conns[n].prevmetrics;
	s->conns[n].prevmetrics += d;
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

unsigned short
server_get_connections(server *s)
{
        return s->nconns;
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

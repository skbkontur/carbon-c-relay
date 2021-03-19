/*
 * Copyright 2013-2018 Michail Safronov
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

#include "eventpipe.h"

#ifndef HAS_EVENTFD
#if __linux && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 7))
#define HAS_EVENTFD
#endif
#endif

#ifdef HAS_EVENTFD
#include <sys/eventfd.h>
#endif

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int eventpipe_init(eventpipe *p) {
    p->fd[0] = -1;
#if HAVE_EVENTFD
    p->fd[1] = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (p->fd[1] < 0 && errno == EINVAL) {
        p->fd[1] = eventfd(0, 0);
    }

    if (fds[1] < 0)
#endif
    {
        return socketpair(AF_LOCAL, SOCK_STREAM, 0, p->fd);
    }
}

int eventpipe_rd(eventpipe *p) {
    if (p->fd[0] < 0) {
        return p->fd[1]; /* eventfd */
    } else {
        return p->fd[0]; /* pipe read descriptor */
    }
}

int eventpipe_wd(eventpipe *p) { return p->fd[1]; }

int eventpipe_close(eventpipe *p) { 
    if (p->fd[0] > 0) {
        close(p->fd[0]);
    }
    return close(p->fd[1]); 
}

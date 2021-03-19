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

/* eventfd/socketpair wrapper for event loop notify */

#ifndef EVENTPIPE_H
#define EVENTPIPE_H 1

struct _eventpipe {
    int fd[2];
};
typedef struct _eventpipe eventpipe;

/* init eventpipe with eventfd or socketpair */
int eventpipe_init(eventpipe *p);

/* return eventpipe read fd */
int eventpipe_rd(eventpipe *p);

/* return eventpipe write fd */
int eventpipe_wd(eventpipe *p);

/* close eventpipe */
int eventpipe_close(eventpipe *p);

#endif

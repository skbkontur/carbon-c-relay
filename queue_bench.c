/* benchmark for queue */
#include <getopt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark_threads.h"
#include "queue.h"

const char *n = "1";

// static read_task *read_tasks;

static queue *q;

int nobench_queue_dequeue(B *b) {
    char *p;

    (void) b_start(b);

    if (b_start_sync(b) != BENCH_SUCCESS) {
        return BENCH_ERROR;
    }

    while (b_running(b)) {
        p = (char *) queue_dequeue(q);
        if (p != NULL) {
            free(p);
        }
    }

    return BENCH_SUCCESS;
}

int bench_queue_enqueue(B *b) {

    /* Do setup here */
    int i;
    int status = BENCH_SUCCESS;

    if (b_start_sync(b) != BENCH_SUCCESS) {
        return BENCH_ERROR;
    }

    /* Start the clock at the last possible second!  */
    b_start_timer(b);

    for (i = 0; i < b_count(b); i++) {
        queue_enqueue(q, strdup(n));
        if (queue_free(q) == 0) {
            fprintf(stderr, "ERROR %s:%d %s: queue overflow\n", __FILE__,
                    __LINE__, (char *) b_key(b));
            status = BENCH_ERROR;
            break;
        }
    }

    /* Stop the clock at the earliest convience */
    b_stop_timer(b);

    /* Do cleanup here */

    return status;
}

void benchmark_queue_enqueue(int count, int writers, int readers) {
    const char *name = "queue_enqueue";

    BENCH_T(count, name, writers, &bench_queue_enqueue, readers, &nobench_queue_dequeue, NULL, NULL, 1);
}

void usage(const char *prog) {
    fprintf(stderr, "use: %s [-r readers] [-w writers]\n", prog);
    exit(-1);
}

int main(int argc, char *const argv[]) {
    int c, count;
    int writers = 1;
    int readers = 1;

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"read", required_argument, NULL, 0},
            {"write", required_argument, NULL, 0},
            {"help", required_argument, NULL, 0},
            {0, 0, NULL, 0}};

        c = getopt_long(argc, argv, "r:w:h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'r':
            readers = atoi(optarg);
            if (readers < 1) {
                fprintf(stderr, "invalid readers\n");
                exit(-1);
            }
            break;
        case 'w':
            writers = atoi(optarg);
            if (writers < 1) {
                fprintf(stderr, "invalid writers\n");
                exit(-1);
            }
            break;
        default:
            usage(argv[0]);
        }
    }
    printf("Read threads: %d, write threads: %d\n", readers, writers);
    b_print_header();

    c = 100000;
    count = c / writers;
    q = queue_new(c);

    benchmark_queue_enqueue(count, writers, readers);

    queue_destroy(q);
    return BENCH_STATUS;
}

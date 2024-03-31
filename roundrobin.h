#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "queue.h"
#include "process.h"

#define TRUE 1
#define FALSE 2

void roundRobin(process_t **processes, queue_t *queue, int num_process, int quantum);

#endif
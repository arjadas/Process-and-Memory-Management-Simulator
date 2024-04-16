#ifndef TASK1_H
#define TASK1_H

#include "queue.h"
#include "process.h"

#define TRUE 1
#define FALSE 0

void round_robin(process_t **processes, queue_t *queue, int num_process, int quantum, int *makespan);

#endif
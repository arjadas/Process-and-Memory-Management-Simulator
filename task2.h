#ifndef TASK2_H
#define TASK2_H

#include "process.h"
#include "memory.h"
#include "queue.h"
#include "bitmap.h"

queue_t *initial_memory_allocation(process_t **process, int *num_processes);
void print_process2(process_t *process);

#endif
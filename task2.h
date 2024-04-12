#ifndef TASK2_H
#define TASK2_H

#include "process.h"
#include "memory.h"
#include "queue.h"
#include "bitmap.h"

void initial_memory_allocation(process_t **processes, int *num_processes, bitmap_t bitmap);
void print_process2(process_t *process);
process_t *get_next_process(queue_t *queue, bitmap_t bitmap);
void scheduler(int quantum, queue_t *queue, bitmap_t bitmap, int *num_processes, process_t **processes);

#endif
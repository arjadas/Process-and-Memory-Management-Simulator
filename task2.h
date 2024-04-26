#ifndef TASK2_H
#define TASK2_H

#include "process.h"
#include "memory.h"
#include "queue.h"
#include "bitmap.h"

#define TRUE 1
#define FALSE 0
#define T2_SIZE 2048

void initial_memory_allocation(process_t **processes, int *num_processes, bitmap_t *bitmap);
void print_process2(process_t *process);
process_t *get_next_process(queue_t *queue, bitmap_t *bitmap);
void scheduler(process_t **processes, queue_t *queue, int num_process, int quantum, unsigned long int *makespan, bitmap_t *bitmap);
#endif
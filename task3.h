#ifndef TASK3_H
#define TASK3_H

#include "process.h"
#include "queue.h"
#include "page.h"

#define TRUE 1
#define FALSE 0
#define T3_SIZE 512

void initial_page_allocation(process_t **processes, allocation_t *allocation);
void print_process3(process_t *process);
void paged_scheduler(process_t **processes, queue_t *queue, int num_processes, int quantum, int *makespan, allocation_t *allocation);
process_t *least_recently_executed(process_t **processes, int num_processes, int time);
void evict_and_allocate(allocation_t *allocation, process_t **processes, int num_processes, process_t *process, int time);
process_t *get_next_paged_process(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes, int time);

#endif
#ifndef TASK4_H
#define TASK4_H

#define MIN_ALLOCATION 4

#include "task3.h"
#include "process.h"
#include "queue.h"
#include "page.h"

void virtual_memory_scheduler(process_t **processes, queue_t *queue, int num_processes, int quantum, unsigned long int *makespan, allocation_t *allocation);
void print_table_virtual(page_table_t *page_table);
process_t *get_next_paged_process_virtual(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes, unsigned long int time);
void allocate_pages_virtual(allocation_t *allocation, page_table_t *page_table, int id);
void evict_and_allocate_virtual(allocation_t *allocation, process_t **processes, int num_processes, process_t *process, unsigned long int time);
void deallocate_allocation_virtual(allocation_t *allocation, page_table_t *page_table, int id, unsigned long int time);

#endif
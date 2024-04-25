#ifndef TASK4_H
#define TASK4_H

#include "task3.h"
#include "process.h"
#include "queue.h"
#include "page.h"

void virtual_memory_scheduler(process_t **processes, queue_t *queue, int num_processes, int quantum, int *makespan, allocation_t *allocation);
void print_table_virtual(page_table_t *page_table);
process_t *get_next_paged_process_virtual(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes, int time);
int allocate_pages_virtual(allocation_t *allocation, page_table_t *page_table, int id);
int evict_and_allocate_virtual(allocation_t *allocation, process_t **processes, int num_processes, process_t *process, int time);
void deallocate_allocation_virtual(allocation_t *allocation, page_table_t *page_table, int id, int time);

#endif
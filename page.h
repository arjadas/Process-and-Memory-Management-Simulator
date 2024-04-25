#ifndef PAGE_H
#define PAGE_H
#include "process.h"

#define PAGE_SIZE 4
#define TRUE 1
#define FALSE 0

typedef struct page_table
{
    int *allocation;       // stores the page numbers of the pages allocated to the process
    int allocated;         // true or false: if pages have been allocated to process or not
    int amount;            // the total number of pages required
    int current_amount;    // the number of pages currently allocated
    int start_frame_index; // the index from which frames are allocated
} page_table_t;

typedef struct page
{
    int id;      // page number
    int evicted; // time that a pages was evicted from a process
} page_t;

typedef struct
{
    page_t **allocations; // stores all the pages that can be allocated
    int vacancies;        // number of available pages
    int size;             // number of pages that can be allocated
} allocation_t;

allocation_t *make_allocation(int size);
int allocate_pages(allocation_t *allocation, page_table_t *page_table, int id);
void deallocate_allocation(allocation_t *allocation, page_table_t *page_table, int id, int time);
void destroy_table(allocation_t *allocation);
void print_table(page_table_t *page_table);
void print_eviction(allocation_t *allocation, int time);
void print_allocation(allocation_t *allocation);

#endif
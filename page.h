#ifndef PAGE_H
#define PAGE_H
#include "process.h"

#define PAGE_SIZE 4
#define TRUE 1

typedef struct page_table
{
    int *allocation;
    int allocated;
    int amount; 
    int current_amount;
} page_table_t;

typedef struct page
{
    int id;
    unsigned long int time;
} page_t;

typedef struct 
{
    page_t **allocations;
    int vacancies;  
    int size;
} allocation_t;

allocation_t *make_allocation(int size);
int allocate_pages(allocation_t *allocation, int memory_KB, page_table_t *page_table, int id, int time);
void deallocate_allocation(allocation_t *allocation, int id);
void destroy_table(allocation_t *allocation);
void print_table(page_table_t *page_table);

#endif
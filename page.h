#ifndef PAGE_H
#define PAGE_H
#include "process.h"

#define PAGE_SIZE 4

typedef struct {
    int *allocations;
    int vacancies;  
    int size;
} page_table_t;

page_table_t *make_page_table(int size);
int allocate_pages(page_table_t *page_table, process_t *process);
void deallocate_page_table(page_table_t *page_table, int id);
void destroy_table(page_table_t *page_table);

#endif
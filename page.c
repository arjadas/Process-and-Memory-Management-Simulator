#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "process.h"
#include "page.h"

page_table_t *make_page_table(int size)
{
    page_table_t *page_table = (page_table_t *)malloc(sizeof(page_table_t));
    page_table->allocations = (int *)malloc(sizeof(int)*size);
    page_table->size = size;
    for (int i = 0; i < size; i++)
    {
        (page_table->allocations)[i] = 0;
    }
    page_table->vacancies = size;
    return page_table;
}

int allocate_pages(page_table_t *page_table, process_t *process)
{
    /*
        in order to allocate memory we search the page table to find vacant pages
    */
    // search for block
    int num_pages = 0;
    num_pages = (process->memory_KB / 4) + !(process->memory_KB % PAGE_SIZE == 0);

    if (page_table->vacancies < num_pages)
    {
        return 0;
    }

    int count = 0;
    process->pages = (int *)malloc(sizeof(int) * num_pages);

    for (int i = 0; (i < page_table->size); i++)
    {
        if (((page_table->allocations)[i]) == -1)
        {
            (process->pages)[count] = i;
            count++;

            (page_table->allocations)[i] = process->id;
            (page_table->vacancies)--;

            if ((count == num_pages)) // found enough pages
            {
                return 1;
            }
        }
    }
    return 0;
}

// deallocate memory
void deallocate_page_table(page_table_t *page_table, int id)
{
    /*
        free memory from the page table
    */
    for (int i = 0; i < page_table->size; i++)
    {
        if ((page_table->allocations)[i] == id)
        {
            (page_table->allocations)[i] = -1;
            (page_table->vacancies)++;
        }
    }
}

// destroy table not necessary as was statically allocated.
void destroy_table(page_table_t *page_table)
{
    free(page_table->allocations);
    free(page_table);
}
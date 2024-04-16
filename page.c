#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "process.h"
#include "page.h"

allocation_t *make_allocation(int size)
{
    allocation_t *allocation = (allocation_t *)malloc(sizeof(allocation_t));
    allocation->allocations = (page_t **)malloc(sizeof(page_t *) * size);
    allocation->size = size;
    for (int i = 0; i < size; i++)
    {
        (allocation->allocations)[i] = (page_t *)malloc(sizeof(page_t));
        (allocation->allocations)[i]->id = -1;
        (allocation->allocations)[i]->time = -1;
    }
    allocation->vacancies = size;
    return allocation;
}

int allocate_pages(allocation_t *allocation, int memory_KB, page_table_t *page_table, int id, int time)
{
    /*
        in order to allocate memory we search the page table to find vacant pages
    */
    // search for block
    int num_pages = 0;
    num_pages = page_table->amount;

    if (allocation->vacancies < num_pages)
    {
        return 0;
    }

    int count = 0;
    page_table->allocation = (int *)malloc(sizeof(int) * num_pages);

    for (int i = 0; (i < allocation->size); i++)
    {
        if (((allocation->allocations)[i]->id) == -1)
        {
            (page_table->allocation)[count] = i;
            count++;

            (allocation->allocations)[i]->id = id;
            (allocation->allocations)[i]->time = time;
            (allocation->vacancies)--;

            if ((count == num_pages)) // found enough pages
            {
                page_table->amount = num_pages;
                page_table->allocated = TRUE;
                page_table->current_amount = num_pages;
                return 1;
            }
        }
    }
    return 0;
}

// deallocate memory
void deallocate_allocation(allocation_t *allocation, int id)
{
    /*
        free memory from the page table
    */
    for (int i = 0; i < allocation->size; i++)
    {
        if ((allocation->allocations)[i]->id == id)
        {
            (allocation->allocations)[i]->id = -1;
            (allocation->allocations)[i]->time = -1;
            (allocation->vacancies)++;
        }
    }
}

// destroy table not necessary as was statically allocated.
void destroy_table(allocation_t *allocation)
{   
    free(allocation->allocations);
    free(allocation);
}

void print_table(page_table_t *page_table)
{
    printf("mem-frames=[");
    for (int i = 0; i < page_table->allocated - 1; i++)
    {
        printf("%d,", page_table->allocation[i]);
    }
    printf("%d]\n", page_table->allocation[page_table->allocated - 1]);
}
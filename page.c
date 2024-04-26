#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "process.h"
#include "page.h"

allocation_t *make_allocation(int size)
{
    /*
        makes an allocation table to map pages to frames
    */

    allocation_t *allocation = (allocation_t *)malloc(sizeof(allocation_t));
    allocation->allocations = (page_t **)malloc(sizeof(page_t *) * size);
    allocation->size = size;
    for (int i = 0; i < size; i++)
    {
        (allocation->allocations)[i] = (page_t *)malloc(sizeof(page_t));
        (allocation->allocations)[i]->id = NOT_SET;
        (allocation->allocations)[i]->evicted = NOT_SET;
    }
    allocation->vacancies = size;
    return allocation;
}

void allocate_pages(allocation_t *allocation, page_table_t *page_table, int id)
{
    /*
        in order to allocate memory we search the page table to find vacant pages
    */

    // search for block, might not be very useful as already making sure before this function is called
    if (allocation->vacancies < page_table->amount)
    {
        return;
    }

    // if pages not allocated, then allocate
    if (page_table->allocation == NULL)
    {
        page_table->allocation = (int *)malloc(sizeof(int) * page_table->amount);
    }

    // find and allocate pages to process
    for (int i = 0; (i < allocation->size); i++)
    {
        if (((allocation->allocations)[i]->id) == NOT_SET)
        {
            (page_table->allocation)[page_table->current_amount] = i;
            (page_table->current_amount)++;
            (allocation->allocations)[i]->id = id;
            (allocation->allocations)[i]->evicted = NOT_SET;
            (allocation->vacancies)--;

            if ((page_table->current_amount == page_table->amount)) // found enough pages, all pages allocated
            {
                page_table->allocated = TRUE;
                return;
            }
        }
    }
}

// deallocate memory
void deallocate_allocation(allocation_t *allocation, page_table_t *page_table, int id, int time)
{
    /*
        free memory from the page table
    */

    // first set pages in allocation block to NOT_SET
    for (int i = 0; i < allocation->size; i++)
    {
        if ((allocation->allocations)[i]->id == id)
        {
            (allocation->allocations)[i]->id = NOT_SET;
            (allocation->allocations)[i]->evicted = time;
            (allocation->vacancies) += 1;
        }
    }

    // then set the allocation array for the process to NOT_SET
    for (int i = 0; i < page_table->amount; i++)
    {
        (page_table->allocation)[i] = NOT_SET;
        (page_table->current_amount)--;
    }
    page_table->allocated = FALSE;
}

void destroy_table(allocation_t *allocation)
/*
    destroy table (i.e. free memory :) )
*/

{
    if (allocation != NULL)
    {
        if (allocation->allocations != NULL)
        {
            for (int i = 0; i < allocation->size; i++)
            {
                if (allocation->allocations[i] != NULL)
                {
                    free(allocation->allocations[i]);
                }
            }
            free(allocation->allocations);
        }
        free(allocation);
    }
}

void print_allocation(allocation_t *allocation)
{
    for (int i = 0; i < allocation->size; i++)
    {
        printf("allocations[i] = %d\n", (allocation->allocations)[i]->id);
    }
}

void print_table(page_table_t *page_table)
{
    printf("mem-frames=[");
    for (int i = 0; i < page_table->amount - 1; i++)
    {
        printf("%d,", page_table->allocation[i]);
    }
    printf("%d]\n", page_table->allocation[page_table->amount - 1]);
}

void print_eviction(allocation_t *allocation, int time)
{
    /*
        prints this: <time>,EVICTED,evicted-frames=<[frames]>
    */

    printf("%d,EVICTED,evicted-frames=[", time);
    int num_printed = 1, num_pages = 0;
    for (int i = 0; i < allocation->size; i++)
    {
        if ((allocation->allocations[i])->evicted == time)
        {
            num_pages++;
        }
    }
    for (int i = 0; i < allocation->size; i++)
    {
        if ((allocation->allocations[i])->evicted == time)
        {
            if (num_printed < num_pages)
            {
                printf("%d,", i);
                num_printed++;
            }
            else if (num_pages == num_printed)
            {
                printf("%d", i);
            }
        }
    }
    printf("]\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "read.h"
#include "process.h"
#include "memory.h"
#include "queue.h"
#include "page.h"
#include "task3.h"
#include "task4.h"

void virtual_memory_scheduler(process_t **processes, queue_t *queue, int num_processes, int quantum, int *makespan, allocation_t *allocation)
{
    /*
        scheduler: allocates processes in queue CPU time for one quantum if they have a memory allocation,
            if not then they are sent to the back of the queue. uses roundrobin algorithm
    */
    int simulation_time = 0;
    int submitted_process = 0;
    int remaining_process = num_processes;
    int ready_process_remaining = 0;
    int should_loop = TRUE;
    float percentage = 0;
    process_t *current_process = NULL;

    while (should_loop == TRUE)
    {
        // step 1: identify and add new processes to the queue
        while (submitted_process < num_processes && processes[submitted_process]->arrival_time <= simulation_time)
        {
            change_status(processes[submitted_process], READY);
            enqueue(queue, processes[submitted_process]);
            submitted_process++;
        }

        // step 2: check if current process has completed its execution

        if (current_process != NULL)
        {
            // if process is finished
            if (current_process->remaining_time <= 0)
            {
                change_status(current_process, FINISHED);

                remaining_process--;
                ready_process_remaining = get_queue_length(queue);

                current_process->completion_time = simulation_time; //  time of completion for the process
                current_process->turnaround_time = simulation_time - current_process->arrival_time;
                current_process->time_overhead = (current_process->turnaround_time * 1.0) / current_process->service_time; // multiply with 1.0 to convert to float/double
                deallocate_allocation(allocation, current_process->page_table, current_process->id, simulation_time);
                print_eviction(allocation, simulation_time);
                printf("%d,%s,process-name=%s,proc-remaining=%d\n", simulation_time, get_status_string(current_process), current_process->name, ready_process_remaining);
                current_process = NULL;

                if (remaining_process == 0 && is_empty(queue))
                {
                    should_loop = FALSE; // if we use a break, we can skip the should_loop variable
                    break;
                } // else loop continues
            }
            else
            {
                // requires more time
                if (!is_empty(queue))
                {
                    change_status(current_process, READY);
                    enqueue(queue, current_process);
                    current_process = NULL;
                } // else loop keeps running, status is not changed
            }
        }

        // step 3: determine the process that runs in this cycle

        if (current_process == NULL && !is_empty(queue))
        {
            current_process = get_next_paged_process_virtual(queue, allocation, processes, num_processes, simulation_time);
            if (current_process != NULL)
            {
                change_status(current_process, RUNNING);
                percentage = ceil((100 * (float)((allocation->size) - (allocation->vacancies)) / allocation->size));
                printf("%d,%s,process-name=%s,remaining-time=%d,mem-usage=%.0f%%,",
                       simulation_time, get_status_string(current_process), current_process->name, current_process->remaining_time,
                       percentage);
                print_table(current_process->page_table);
            }
        }

        if (current_process != NULL && current_process->remaining_time > 0)
        {
            (current_process->remaining_time -= quantum);
        }

        // update simulation time
        simulation_time += quantum;
    }

    *makespan = simulation_time;
}

process_t *get_next_paged_process_virtual(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes, int time)
{
    /*
        get next process that has been allocated memory, if no memory allocation and
            memory allocation not possible then return NULL

    */
    process_t *process = NULL;
    process = dequeue(queue);
    if (process->page_table->allocated != TRUE)
    {
        // first check if there are 4 free pages in the allocation block
        if ((allocation->vacancies) >= 4 || (process->page_table->amount < 4 && (allocation->vacancies) >= (process->page_table->amount))) // if n=3, then does it have more than 3 frames ?
        {
            allocate_pages_virtual(allocation, process->page_table, process->id);
        }
        else // allocation->vacancies < 4 or not met
        {
            evict_and_allocate_virtual(allocation, processes, num_processes, process, time);
        }
    }
    return process;
}

int allocate_pages_virtual(allocation_t *allocation, page_table_t *page_table, int id)
{
    /*
        in order to allocate memory we search the page table to find vacant pages
    */
    // search for block
    /*
    if (allocation->vacancies < page_table->amount)
    {
        return 0; // i see no use of this condition
    }*/

    if (page_table->allocation == NULL)
    {
        page_table->allocation = (int *)malloc(sizeof(int) * page_table->amount);
    }

    // no change needed as it will only allocate frames until it can
    for (int i = 0; (i < allocation->size); i++)
    {
        if (((allocation->allocations)[i]->id) == -1)
        {
            (page_table->allocation)[page_table->current_amount] = i;
            (page_table->current_amount)++;
            (allocation->allocations)[i]->id = id;
            (allocation->allocations)[i]->evicted = -1;
            (allocation->vacancies)--;

            if ((page_table->amount >= 4 && page_table->current_amount >= 4) || (page_table->amount < 4 && page_table->current_amount == page_table->amount)) // found enough pages
            {
                page_table->allocated = TRUE;
                return 1;
            }
        }
    }
    return 0;
}

int evict_and_allocate_virtual(allocation_t *allocation, process_t **processes, int num_processes, process_t *process, int time)
{
    // step 1: evict process from pages and allocate pages to new process
    process_t *evicted = NULL;
    while (allocation->vacancies < 4)
    {
        // step 2: find page that was least recently executed
        evicted = least_recently_executed(processes, num_processes, time);
        assert(evicted);

        // step 3: evict process from pages and allocate pages to new process

        if (evicted->page_table->amount < 4)
        {
            deallocate_allocation(allocation, evicted->page_table, evicted->id, time);
        }
        else
        {
            deallocate_allocation_virtual(allocation, evicted->page_table, evicted->id, time);
        }
    }
    print_eviction(allocation, time);

    // now we have enough pages available
    allocate_pages(allocation, process->page_table, process->id);
    return 0;
}

void deallocate_allocation_virtual(allocation_t *allocation, page_table_t *page_table, int id, int time)
{
    int count = 0;
    /*
        free memory from the page table
    */
    // first set pages in allocation block to -1
    for (int i = 0; i < allocation->size && allocation->vacancies < 4; i++) // here, i represents the frame number in allocation right?
    {
        if ((allocation->allocations)[i]->id == id)
        {
            (allocation->allocations)[i]->id = -1;
            (allocation->allocations)[i]->evicted = time;
            (allocation->vacancies) += 1;
            count++;
        }

        if ((page_table->allocation)[i] == (allocation->allocations)[i]->id)
        {
        }
    }

    // then set the allocation array for the process to -1
    for (int i = 0; i < page_table->amount && i < count; i++)
    {
        (page_table->allocation)[i] = -1;
        (page_table->current_amount)--;
    }
    page_table->allocated = FALSE;
}
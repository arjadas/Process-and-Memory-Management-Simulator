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

void initial_page_allocation(process_t **processes, allocation_t *allocation)
{
    /*
        allocate memory to the first process
    */
    allocate_pages(allocation, processes[0]->page_table, processes[0]->id);
}

void print_process3(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %lu\n\tService time: %lu\n\tMemory required: %d\n Amount: %d\n",
           process->name, process->arrival_time, process->service_time, process->memory_KB, process->page_table->amount);
}

void paged_scheduler(process_t **processes, queue_t *queue, int num_processes, int quantum, int *makespan, allocation_t *allocation)
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
            current_process = get_next_paged_process(queue, allocation, processes, num_processes, simulation_time);
            if (current_process != NULL)
            {
                change_status(current_process, RUNNING);

                percentage = ceil((100 * (float)((allocation->size) - (allocation->vacancies)) / allocation->size));
                printf("%d,%s,process-name=%s,remaining-time=%lu,mem-usage=%.0f%%,",
                       simulation_time, get_status_string(current_process), current_process->name, current_process->remaining_time,
                       percentage);
                print_table(current_process->page_table);
            }
        }

        if (current_process != NULL && current_process->remaining_time > 0)
        {
            (current_process->remaining_time -= quantum);
            current_process->last_executed = simulation_time;
        }

        // update simulation time
        simulation_time += quantum;
    }

    *makespan = simulation_time;
}

void evict_and_allocate(allocation_t *allocation, process_t **processes, int num_processes, process_t *process, int time)
{
    // step 1: evict process from pages and allocate pages to new process
    process_t *evicted = NULL;
    while (allocation->vacancies < process->page_table->amount)
    {
        // step 2: find page that was least recently executed
        evicted = least_recently_executed(processes, num_processes, time);
        assert(evicted);

        // step 3: evict process from pages and allocate pages to new process
        deallocate_allocation(allocation, evicted->page_table, evicted->id, time);
    }
    print_eviction(allocation, time);

    // now we have enough pages available
    allocate_pages(allocation, process->page_table, process->id);
}

process_t *least_recently_executed(process_t **processes, int num_processes, int time)
{
    /*
        linear search to find the least recently executed
    */

    int current_min = time;
    process_t *temp = processes[0];
    assert(processes);
    for (int i = 0; i < num_processes; i++)
    {
        if ((current_min > (processes[i])->last_executed) && (processes[i]->page_table->allocated == TRUE))
        {
            temp = processes[i];
            current_min = (processes[i])->last_executed;
        }
    }
    return temp;
}

process_t *get_next_paged_process(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes, int time)
{
    /*
        get next process that has been allocated memory, if no memory allocation and
            memory allocation not possible then return NULL
    */

    process_t *process = NULL;
    process = dequeue(queue);
    if (process->page_table->allocated != TRUE)
    {
        // first check if there are enough free pages in the allocation block
        if ((allocation->vacancies) >= (process->page_table->amount))
        {
            allocate_pages(allocation, process->page_table, process->id);
        }
        else
        {
            evict_and_allocate(allocation, processes, num_processes, process, time);
        }
    }
    return process;
}
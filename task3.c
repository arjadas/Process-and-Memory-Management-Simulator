#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
   int time = 0;
    allocate_pages(allocation, processes[0]->memory_KB, processes[0]->page_table, processes[0]->id, time);
} 

void print_process3(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %d\n\tService time: %lu\n\tMemory required: %d\n Memory allocation: %d, %d\n",
           process->name, process->arrival_time, process->service_time, process->memory_KB, process->page_table->allocation[0], process->page_table->allocation[1]);
           print_table(process->page_table);
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

        // a cycle of looping

        // step 2: check if current process has completed its execution

        if (current_process != NULL)
        {
            if (current_process->remaining_time <= 0)
            {
                change_status(current_process, FINISHED);

                remaining_process--;
                ready_process_remaining = get_queue_length(queue);
                printf("%d,%s,process-name=%s,proc-remaining=%d\n", simulation_time, get_status_string(current_process), current_process->name, ready_process_remaining);

                current_process->completion_time = simulation_time; //  time of completion for the process
                current_process->turnaround_time = simulation_time - current_process->arrival_time;
                current_process->time_overhead = (current_process->turnaround_time * 1.0) / current_process->service_time; // multiply with 1.0 to convert to float/double
                deallocate_allocation(allocation, current_process->id);
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
            current_process = get_next_paged_process(queue, allocation, processes, num_processes);
            if (current_process != NULL)
            {
                change_status(current_process, RUNNING);
                percentage = ((float)((allocation->size) - (allocation->vacancies))/ allocation->size)*100;
                printf("%d,%s,process-name=%s,remaining-time=%d,mem-usage=%.f%%",
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

int evict_and_allocate(allocation_t *allocation, process_t **processes, int num_processes, process_t *process)
{
    // step 2: evict process from pages and allocate pages to new process
    process_t *evicted = NULL;
    int page = -1, current_amount = -1;
    while (process->page_table->allocated != TRUE)
    {
        // step 1: find page that was least recently executed
        evicted = least_recently_executed(processes, num_processes);

        // step 2: evict process from pages and allocate pages to new process
        for (int i = 0; (i < evicted->page_table->amount) && (process->page_table->current_amount <= process->page_table->amount); i++)
        {
            page = (evicted->page_table->allocation)[i];
            current_amount = process->page_table->current_amount;
            (process->page_table->allocation)[i + current_amount] = page;
            allocation->allocations[page]->id = process->id;
            (evicted->page_table->allocation)[i] = -1;
            (process->page_table->current_amount)++;
        }
        if (process->page_table->current_amount == process->page_table->amount)
        {
            process->page_table->allocated = TRUE;
            break;
            return 1;
        }
    }
    return 0;
}

process_t *least_recently_executed(process_t **processes, int num_processes)
{
    int current_min = 0;
    process_t *temp = processes[0];
    assert(processes);
    for (int i = 0; i < num_processes; i++)
    {
        if ((current_min > (processes[i])->last_executed) && (processes[i]->page_table->allocated == TRUE))
        {
            temp = processes[i];
        }
    }
    return temp;
}

process_t *get_next_paged_process(queue_t *queue, allocation_t *allocation, process_t **processes, int num_processes)
{
    /*
        get next process that has been allocated memory, if no memory allocation and
            memory allocation not possible then return NULL

    */
    process_t *process = NULL;
    process = dequeue(queue);
    if (process->page_table->allocated != TRUE)
    {
        evict_and_allocate(allocation, processes, num_processes, process);
    }
    return process;
}
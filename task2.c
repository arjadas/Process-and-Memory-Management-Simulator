#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"
#include "memory.h"
#include "queue.h"
#include "bitmap.h"
#include "task2.h"

void initial_memory_allocation(process_t **processes, int *num_processes, bitmap_t *bitmap)
{
    /*
        allocate memory to the first process
    */
    allocate_memory(bitmap, processes[0]);
}

void print_process2(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %lu\n\tService time: %lu\n\tMemory required: %d\n Memory allocation: %d to %d amount = %d\n",
           process->name, process->arrival_time, process->service_time, process->memory_KB, process->allocation->start, process->allocation->end, process->allocation->quantity);
}

void scheduler(process_t **processes, queue_t *queue, int num_process, int quantum, int *makespan, bitmap_t *bitmap)
{
    /*
        scheduler: allocates processes in queue CPU time for one quantum if they have a memory allocation,
            if not then they are sent to the back of the queue. uses roundrobin algorithm
    */

    int simulation_time = 0;
    int submitted_process = 0;
    int remaining_process = num_process;
    int ready_process_remaining = 0;
    int should_loop = TRUE;
    process_t *current_process = NULL;

    while (should_loop == TRUE)
    {
        // step 1: identify and add new processes to the queue
        while (submitted_process < num_process && processes[submitted_process]->arrival_time <= simulation_time)
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
                deallocate_bitmap(bitmap, current_process->allocation);
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
            current_process = get_next_process(queue, bitmap);
            if (current_process != NULL)
            {
                change_status(current_process, RUNNING);

                printf("%d,%s,process-name=%s,remaining-time=%lu,mem-usage=%.f%%,allocated-at=%d\n",
                       simulation_time, get_status_string(current_process), current_process->name, current_process->remaining_time,
                       ((float)(bitmap->allocated) / T2_SIZE) * 100, current_process->allocation->start); // multiply by 100 for conversion to percentage
            }
        }

        if (current_process != NULL && current_process->remaining_time > 0)
        {
            if (current_process->remaining_time >= quantum)
                (current_process->remaining_time -= quantum);
            else
                current_process->remaining_time = 0;
        }

        // update simulation time
        simulation_time += quantum;
    }

    *makespan = simulation_time;
}

process_t *get_next_process(queue_t *queue, bitmap_t *bitmap)
{
    /*
        get next process that has been allocated memory, if no memory allocation and
            memory allocation not possible then return NULL
    */

    process_t *process = NULL, *temp = NULL;
    int i = 0;
    int allocate = 0;
    while (i < get_queue_length(queue))
    {
        temp = dequeue(queue);
        assert(temp != NULL);

        if (temp->allocation->quantity == temp->memory_KB)
        {
            process = temp;
            break;
        }
        else
        {
            allocate = allocate_memory(bitmap, temp);
            if (allocate == TRUE)
            {
                // allocation was successful
                process = temp;
                break;
            }
            else
            {
                enqueue(queue, temp);
            }
        }
    }
    return process;
}
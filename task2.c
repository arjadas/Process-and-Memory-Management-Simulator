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

void initial_memory_allocation(process_t **processes, int *num_processes, bitmap_t bitmap)
{
    /*
        make a linked list and allocate memory to the processes
    */

    for (int i = 0; i < *num_processes; i++)
    {
        // enqueue node
        printf("inside the for loop line 23 i = %d\n", i);

        // allocate memory (if possible)
        allocate_memory(bitmap, processes[i]);
    }
    for (int i = 0; i < *num_processes; i++)
    {
        printf("for loop line 31 i = %d\n", i);
        print_process2(processes[i]);
    } 
    printf("here\n");
}

void print_process2(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %d\n\tService time: %lu\n\tMemory required: %d\n Memory allocation: %d to %d\n", 
        process->name, process->arrival_time, process->service_time, process->memory_KB, process->allocation->start, process->allocation->end);
}

void scheduler(int quantum, queue_t *queue, bitmap_t bitmap, int *num_processes, process_t **processes) 
{
    /*
        scheduler: allocates processes in queue CPU time for one quantum if they have a memory allocation, 
            if not then they are sent to the back of the queue. uses roundrobin algorithm
    */
   printf("made it to line 46\n");
    int submitted_processes = 0, completed_processes = 0;
    int remaining_process = *num_processes;
    int simulation_time = 0, should_loop = 1;
    process_t *running_process = NULL, *temp = NULL;
    
    // get first item to put on CPU
    printf("made it to line 52\n");
    running_process = processes[submitted_processes];
    printf("made it to line 54\n");
    // begin scheduler
    while (should_loop && (simulation_time += quantum) && (completed_processes < *num_processes)) 
    {
        // step 1: check for new processes
        print_process2(running_process);
        running_process->remaining_time -= quantum;
        printf("line 62, submitted_processes = %d\n", submitted_processes);
        temp = processes[submitted_processes];
        submitted_processes++;
        printf("made it to line 61, submitted_processes = %d, num_processes = %d\n", submitted_processes, *num_processes);
        while ((temp != NULL) && (temp->arrival_time <= simulation_time) && (submitted_processes < *num_processes))
        {
            // add to the queue
            printf("entered loop\n");
            enqueue(queue, temp);
            temp = processes[submitted_processes];
            submitted_processes++;
        }
        temp = NULL;
        printf("made it to line 73, submitted_processes = %d\n", submitted_processes);
        // step 2: find next process to allocate to the CPU
        if (running_process)
        {   
            if (!isEmpty(queue) && (temp = get_next_process(queue, bitmap)))
            {
                // need to take process off of the CPU
                enqueue(queue, running_process);
                change_status(running_process, READY);

                // get process to give CPU time
                running_process = temp;
                change_status(running_process, RUNNING);
                temp = NULL;
            }
            else
            {
                // if queue is empty then check if process needs to keep running
                if (running_process->remaining_time <= 0)
                {
                    change_status(running_process, FINISHED);
                    printf("%d, %s, process-name = %s, proc-remaining = %d\n", simulation_time, get_status_string(running_process), running_process->name, remaining_process);

                    running_process->completion_time = simulation_time; //  time of completion for the process
                    running_process->turnaround_time = simulation_time - running_process->arrival_time;
                    running_process->time_overhead = (running_process->turnaround_time * 1.0) / running_process->service_time; // multiply with 1.0 to convert to float/double
                    running_process = NULL;
                    completed_processes++;

                    // loop is now done
                    should_loop = 0;
                }
                // running process just gets the CPU for another loop
            }
        }
   }
}

process_t *get_next_process(queue_t *queue, bitmap_t bitmap)
{
    /*
        get next process that has been allocated memory, if no memory allocation and 
            memory allocation not possible then return NULL
    
    */
    process_t *process = NULL, *temp = NULL;
    int i = 0;
    int allocate = 0;
    while (i < queue->length)
    {
        temp = dequeue(queue);
        if (temp->memory_KB)
        {
            process = temp;
        }
        else
        {
            allocate = allocate_memory(bitmap, temp);
            if (allocate)
            {
                // allocation was successful
                process = temp;
            }
        }
    }
    return process;
}
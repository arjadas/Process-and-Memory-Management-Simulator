#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"
#include "memory.h"
#include "queue.h"
#include "bitmap.h"

queue_t *initial_memory_allocation(process_t **processes, int *num_processes)
{
    /*
        make a linked list and allocate memory to the processes
    */
    queue_t *queue = createQueue();
    int *bitmap = make_bitmap();

    for (int i = 0; i < *num_processes; i++)
    {
        // enqueue node
        enqueue(queue, processes[i]);

        // allocate memory (if possible)
        allocate_memory(bitmap, processes[i]);
    }
    for (int i = 0; i < *num_processes; i++)
    {
        print_process2(processes[i]);
    } 
    return queue;
}

void print_process2(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %d\n\tService time: %lu\n\tMemory required: %d\n Memory allocation: %d to %d\n", 
        process->name, process->arrival_time, process->service_time, process->memory_KB, process->allocation.start, process->allocation.end);
}
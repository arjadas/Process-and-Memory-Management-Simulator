#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


process_t *create_process()
{
    /* 
        makes a process by taking in necesary input and allocating memory to store 
        the process's information
    */

    process_t *process = (process_t *)malloc(sizeof(process_t));
    assert(process);
    return process;
}

void print_processes(process_t **processes, int num_processes)
{
    for (int i = 0; i < num_processes; i++)
    {
        print_process(processes[i]);
    }
    
}

void print_process(process_t *process)
{
    printf("Process name: %s\n \tArrival time: %d\n\tService time: %lu\n\tMemory required: %d\n", 
        process->name, process->arrival, process->service_time, process->memory_KB);
}

int change_status(process_t *process, int new_status)
{
    /*
        function updates the status of the process to new_status
        returns -1 if trying to change the status of process to an invalid status
        otherwise, returns the value of new_status
    */

    int status = process->status;
    if (status == FINISHED && new_status != FINISHED) return -1;
    if (status == READY && new_status == FINISHED) return -1;
    if (status == READY && new_status == RUNNING) process->status = new_status;
    if (status == RUNNING && new_status == READY) process->status = new_status;
    if (status == RUNNING && new_status == FINISHED) process->status = new_status;
    return new_status;
}

int get_status(process_t *process)
{
    return process->status;
}

void free_process(process_t *process)
{
    /*
        frees the string stored in process then frees the process itself
    */
    if (process != NULL) 
    {
        if (process->name != NULL) free(process->name);
        free(process);
    }
}
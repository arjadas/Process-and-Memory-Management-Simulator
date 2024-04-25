#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "page.h"

process_t *create_process()
{
    /*
        makes a process by taking in necesary input and allocating memory to store
        the process's information
        char *name;
    int id;
    int arrival_time;
    int remaining_time;
    int last_executed;
    unsigned long int service_time; // check data type
    int completion_time;
    int turnaround_time;
    double time_overhead; // data type - double
    int memory_KB;
    int status;
    memory_t *allocation;
    page_table_t *page_table;
    */

    process_t *process = (process_t *)malloc(sizeof(process_t));
    assert(process);
    process->allocation = NULL;
    process->page_table = NULL;
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
           process->name, process->arrival_time, process->service_time, process->memory_KB);
}

int change_status(process_t *process, int new_status)
{
    /*
        function updates the status of the process to new_status
        returns -1 if trying to change the status of process to an invalid status
        otherwise, returns the value of new_status
    */

    int status = process->status;

    // very first initialisation
    if (status == NOT_SET)
        process->status = new_status;

    if (status == FINISHED && new_status != FINISHED)
        return -1;

    if (status == READY && new_status == FINISHED)
        return -1;

    if (status == READY && new_status == RUNNING)
        process->status = new_status;

    if (status == RUNNING && new_status == READY)
        process->status = new_status;

    if (status == RUNNING && new_status == FINISHED)
        process->status = new_status;

    return new_status;
}

int get_status(process_t *process)
{
    return process->status;
}

char *get_status_string(process_t *process)
{
    switch (process->status)
    {
    case READY:
        return "READY";
        break;

    case RUNNING:
        return "RUNNING";
        break;

    case FINISHED:
        return "FINISHED";
        break;

    default:
        return "UNKNOWN";
    }
}

void free_process(process_t *process)
{
    /*
        frees the string stored in process then frees the process itself
    */

    if (process != NULL)
    {
        if (process->name != NULL)
            free(process->name);
        if (process->allocation != NULL)
        {
            free(process->allocation);
        }
        if (process->page_table != NULL)
        {
            free(process->page_table->allocation);
            free(process->page_table);
        }
        free(process);
    }
}

void free_processes(process_t **processes, int num_processes)
{
    for (int i = 0; i < num_processes; i++)
    {
        free_process(processes[i]);
    }
    free(processes);
}
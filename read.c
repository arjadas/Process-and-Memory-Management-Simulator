#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"
#include "memory.h"
#include "page.h"
#include "task4.h"

process_t **read_processes(read_t *input, int *num_processes)
{
    /*
        reads in processes from filename
        returns an array of processes
    */

    process_t **processes = (process_t **)malloc(sizeof(process_t *) * 100);
    assert(processes != NULL);
    process_t *process = create_process();
    int curr_array_size = 100;

    FILE *fpt = fopen(input->filename, "r");
    assert(fpt);

    enum Input column = ARRIVAL;
    char *buffer = (char *)calloc(10, sizeof(char));
    int i = 0;
    char letter = '\0';

    while ((letter = fgetc(fpt)) != EOF)
    {
        // if character is ' ' or '\n' then we are at the end of an input
        if ((letter == '\n') && (column == MEMORY))
        {
            parse_value(process, buffer, column);
            assign_memory(process, input->memory);
            process->id = *num_processes;
            processes[*num_processes] = process;
            (*num_processes)++;
            process = create_process();
            reset_buffer(buffer, &i);
            column = ARRIVAL;
            if (*num_processes == curr_array_size)
            {
                processes = resize_processes(processes, &curr_array_size);
            }
        }
        else if (letter == ' ') // length of buffer
        {
            add_buffer(buffer, i, letter);
            parse_value(process, buffer, column);
            reset_buffer(buffer, &i);
            column++;
        }
        else
        {
            add_buffer(buffer, i, letter);
            i++;
        }
    }
    free(buffer);
    free(process);
    fclose(fpt);
    return processes;
}

process_t **resize_processes(process_t **processes, int *size)
{
    int new_size = 2 * (*size);
    process_t **new_array = (process_t **)malloc(sizeof(process_t *) * (new_size));
    for (size_t i = 0; i < new_size; i++)
    {
        new_array[i] = processes[i];
    }
    *size = new_size;
    return new_array;
}

void add_buffer(char *buffer, int i, char letter)
{
    /*
        only want to add letter to buffer if it is not white space
    */
    if ((letter != '\n') && (letter != ' ') && (letter != EOF))
    {
        buffer[i] = letter;
    }
}

void parse_value(process_t *process, char *buffer, enum Input column)
{
    switch (column)
    {
    case ARRIVAL:
        /* if arrival time, then turn into int and add to process */
        int arrival = atoi(buffer);
        process->arrival_time = arrival;
        break;
    case NAME:
        process->name = strdup(buffer);
        break;
    case SERVICE_TIME:
        char *writeoff = NULL;
        unsigned long int service_time = strtoul(buffer, &writeoff, 10);
        process->service_time = service_time;

        // remaining time is equal to service time initially
        process->remaining_time = service_time;

        // setting status and completion_time to not_set  question- is it better to keep it 0 or not_set?
        process->completion_time = NOT_SET;
        process->turnaround_time = NOT_SET;
        process->time_overhead = NOT_SET;
        process->status = NOT_SET;
        process->last_executed = NOT_SET;
        break;
    case MEMORY:
        int memory = atoi(buffer);
        process->memory_KB = memory;
        break;
    default:
        break;
    }
}

void assign_memory(process_t *process, int management)
{
    /*
        allocates memory depending on the memory management strategy
    */
    int pages = 0;

    if (management == FIRST_FIT)
    {
        process->allocation = (memory_t *)malloc(sizeof(memory_t));
        process->allocation->quantity = 0;
        process->page_table = NULL;
    }
    else if (management == INFINITE)
    {
        process->page_table = NULL;
        process->allocation = NULL;
    }
    else
    {
        process->allocation = NULL;
        process->page_table = (page_table_t *)malloc(sizeof(page_table_t));
        if (process->memory_KB % MIN_ALLOCATION == 0)
        {
            pages = (process->memory_KB / MIN_ALLOCATION);
        }
        else
        {
            pages = (process->memory_KB / MIN_ALLOCATION) + 1;
        }
        process->page_table->allocation = (int *)malloc(sizeof(int) * pages);
        process->page_table->amount = pages;
        process->page_table->current_amount = 0;
        process->page_table->allocated = FALSE;
        process->page_table->start_frame_index = 0;
        process->page_table->available_start_index = NOT_SET;

        // set the ids to NOT_SET
        for (int i = 0; i < process->page_table->amount; i++)
        {
            process->page_table->allocation[i] = NOT_SET;
        }
    }
}

void reset_buffer(char *buffer, int *length)
{
    for (int i = 0; i < *length; i++)
    {
        buffer[i] = '\0';
    }
    *length = 0;
}

read_t *process_arguments(int argc, char const *argv[])
{
    /*
        takes in command line arguments and returns a struct containing
            - filename of with processes
            - memory strategy to use
            - quantum
    */
    read_t *inputs = (read_t *)malloc(sizeof(read_t));
    assert(inputs);

    for (size_t i = 0; i < argc; i++)
    {
        if (argv[i][1] == 'f')
        {
            inputs->filename = strdup(argv[i + 1]);
            assert(inputs->filename);
        }
        else if (argv[i][1] == 'm')
        {
            inputs->memory = get_strategy(argv[i + 1]);
            assert(inputs->memory != INVALID);
        }
        else if (argv[i][1] == 'q')
        {
            inputs->quantum = atoi(argv[i + 1]);
        }
    }
    return inputs;
}

int get_strategy(const char *strategy)
{
    /*
        finds which search strategy we should use:
            - infinite
            - first fit
            - paged
            - virtual
    */
    char infinite[] = "infinite";
    char first_fit[] = "first-fit";
    char paged[] = "paged";
    char virtual[] = "virtual";
    if (strcmp(strategy, infinite) == 0)
        return INFINITE;
    if (strcmp(strategy, paged) == 0)
        return PAGED;
    if (strcmp(strategy, first_fit) == 0)
        return FIRST_FIT;
    if (strcmp(strategy, virtual) == 0)
        return VIRTUAL;

    return INVALID;
}

void free_inputs(read_t *inputs)
{
    if (inputs != NULL)
    {
        if (inputs->filename != NULL)
        {
            free(inputs->filename);
        }
        free(inputs);
    }
}
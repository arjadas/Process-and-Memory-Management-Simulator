#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"
#include "memory.h"

process_t **read_processes(read_t *input, int *num_processes)
{
    /*
        reads in processes from filename
        returns an array of processes
    */

    process_t **processes = (process_t **)malloc(sizeof(process_t *) * 100);
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

        // setting status and completion_time to not_set
        process->completion_time = NOT_SET;
        process->turnaround_time = NOT_SET;
        process->time_overhead = NOT_SET;
        process->status = NOT_SET;
        break;
    case MEMORY:
        int memory = atoi(buffer);
        process->memory_KB = memory;
        break;
    default:
        break;
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
            inputs->filename = (char *)malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
            assert(inputs->filename);
            strcpy(inputs->filename, argv[i + 1]);
            assert(inputs->filename);
        }
        else if (argv[i][1] == 'm')
        {
            inputs->memory = get_strategy(argv[i + 1]);
            assert(inputs->memory != -1);
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
    char first_fit[] = "first fit";
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

    return -1;
}

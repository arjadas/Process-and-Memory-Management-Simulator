#include "read.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
            inputs->filename = (char *)malloc(sizeof(char) * (strlen(argv[i+1]) + 1));
            assert(inputs->filename);
            strcpy(inputs->filename, argv[i+1]);
            assert(inputs->filename);
        }
        else if (argv[i][1] == 'm')
        {
            inputs->memory = get_strategy(argv[i+1]);
            assert(inputs->memory != -1);
        }
        else if (argv[i][1] == 'q')
        {
            inputs->quantum = atoi(argv[i+1]);
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
    if (strcmp(strategy, infinite) == 0) return INFINITE;
    if (strcmp(strategy, paged) == 0) return PAGED;
    if (strcmp(strategy, first_fit) == 0) return FIRST_FIT;
    if (strcmp(strategy, virtual) == 0) return VIRTUAL;

    return -1;
}



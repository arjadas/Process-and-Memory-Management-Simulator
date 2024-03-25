#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <read.h>

// get arguments

read_t *get_inputs(char const *argv[])
{
    // make read_t
    read_t *inputs = (read_t *)malloc(sizeof(read_t));
    assert(inputs);

    for (size_t i = 0; i < 6; i++)
    {
        if (argv[i][1] == 'f')
        {
            inputs->filename = (char *)malloc(sizeof(char) * (strlen(argv[i+1]) + 1));
            assert(inputs->filename);
            strcpy(inputs->filename, argv[i+1]);
            assert(inputs->filename);
            printf("filename: %s\n", inputs->filename);
        }
        else if (argv[i][1] == 'm')
        {
            inputs->memory = get_strategy(argv[i+1]);
            printf("memory: %d\n", inputs->memory);
        }
        else if (argv[i][1] == 'q')
        {
            inputs->quantum = atoi(argv[i+1]);
            printf("quantum: %d\n", inputs->quantum);
        }
    }
    return inputs;
}

int get_strategy(char *strategy)
{
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



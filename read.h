#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define INFINITE 0
#define FIRST_FIT 1
#define PAGED 2
#define VIRTUAL 3

typedef struct
{
    char *filename;
    int memory;
    int quantum;
} read_t;

read_t *process_arguments(int argc, char const *argv[]);
int get_strategy(char *strategy);

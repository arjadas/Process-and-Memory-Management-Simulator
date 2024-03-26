#ifndef READ_H
#define READ_H

#include <stdio.h>
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

FILE *open_file(char *file_path);
read_t *process_arguments(int argc, char const *argv[]);
int get_strategy(const char *strategy);

#endif
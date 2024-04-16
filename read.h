#ifndef READ_H
#define READ_H
#include <stdio.h>
#include "process.h"

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

enum Input
{
    ARRIVAL,
    NAME,
    SERVICE_TIME,
    MEMORY
};

FILE *open_file(char *file_path);
read_t *process_arguments(int argc, char const *argv[]);
int get_strategy(const char *strategy);
process_t **read_processes(read_t *input, int *num_processes);
void reset_buffer(char *buffer, int *length);
void parse_value(process_t *process, char *buffer, enum Input column);
void add_buffer(char *buffer, int i, char letter);
process_t **resize_processes(process_t **processes, int *size);
void free_inputs(read_t *inputs);
void assign_memory(process_t *process, int management);

#endif
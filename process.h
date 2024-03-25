#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define FINISHED 0
#define READY 1
#define RUNNING 2

typedef struct {
    char *name;
    int arrival;
    int time_required; // check data type
    int memory_KB;
    int status;
} process_t;

process_t *create_process(char *name, int arrival, int time_required, int memory_KB);
int change_status(process_t *process, int new_status);
int get_status(process_t *process);
void free_process(process_t *process);
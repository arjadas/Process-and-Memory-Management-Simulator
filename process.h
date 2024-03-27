#ifndef PROCESS_H
#define PROCESS_H 
#define FINISHED 0
#define READY 1
#define RUNNING 2

typedef struct {
    char *name;
    int arrival;
    int service_time; // check data type
    int memory_KB;
    int status;
} process_t;

process_t *create_process();
int change_status(process_t *process, int new_status);
int get_status(process_t *process);
void free_process(process_t *process);
void print_process(process_t *process);
void print_processes(process_t **processes, int num_processes);

#endif

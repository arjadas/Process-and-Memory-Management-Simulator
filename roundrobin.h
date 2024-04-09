#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "queue.h"
#include "process.h"
#include "queue.h"

#define TRUE 1
#define FALSE 2

void round_robin(process_t **processes, queue_t *queue, int num_process, int quantum, int *makespan);
void print_statistics(process_t **processes, int num_process, int makespan);
int get_average_turnaround(process_t **processes, int num_process);
double get_max_time_overhead(process_t **processes, int num_process);
double get_average_time_overhead(process_t **processes, int num_process);

#endif
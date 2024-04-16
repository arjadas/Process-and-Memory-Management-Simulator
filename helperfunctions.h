#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "process.h"

void print_statistics(process_t **processes, int num_process, unsigned long int makespan);
int get_average_turnaround(process_t **processes, int num_process);
double get_max_time_overhead(process_t **processes, int num_process);
double get_average_time_overhead(process_t **processes, int num_process);

#endif
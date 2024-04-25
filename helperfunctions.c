#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helperfunctions.h"

void print_statistics(process_t **processes, int num_process, int makespan)
{
    printf("Turnaround time %d\n", get_average_turnaround(processes, num_process));

    printf("Time overhead %.2lf %.2lf\n", get_max_time_overhead(processes, num_process), get_average_time_overhead(processes, num_process));

    printf("Makespan %d\n", makespan);
}

int get_average_turnaround(process_t **processes, int num_process)
{

    unsigned long int total = 0;

    for (int i = 0; i < num_process; i++)
    {
        total += processes[i]->turnaround_time;
    }

    return (round((total * 1.0) / num_process)); // multiply with 1.0 to convert to float before rounding up
}

double get_max_time_overhead(process_t **processes, int num_process)
{
    double max = 0;

    for (int i = 0; i < num_process; i++)
    {
        if (processes[i]->time_overhead > max)
        {
            max = processes[i]->time_overhead;
        }
    }

    return roundf(max * 100) / 100;
}

double get_average_time_overhead(process_t **processes, int num_process)
{
    double total = 0;

    for (int i = 0; i < num_process; i++)
    {
        total += processes[i]->time_overhead;
    }

    return roundf((total / num_process) * 100) / 100;
}

/*
    got the rounding method from stack overflow. link given below:
    https://stackoverflow.com/questions/1343890/how-do-i-restrict-a-float-value-to-only-two-places-after-the-decimal-point-in-c/1344261#1344261
*/
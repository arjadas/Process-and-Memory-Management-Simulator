#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "roundrobin.h"
#include "process.h"
#include "queue.h"

void round_robin(process_t **processes, queue_t *queue, int num_process, int quantum, int *makespan)
{
    int simulation_time = 0;
    int submitted_process = 0;
    int remaining_process = num_process;
    int should_loop = TRUE;
    process_t *current_process = NULL;

    while (should_loop == TRUE)
    {
        // step 1: identify and add new processes to the queue
        while (submitted_process < num_process && processes[submitted_process]->arrival_time <= simulation_time)
        {
            change_status(processes[submitted_process], READY);
            enqueue(queue, processes[submitted_process]);
            submitted_process++;
        }

        // a cycle of looping

        // step 2: check if current process has completed its execution

        if (current_process != NULL)
        {
            if (current_process->remaining_time <= 0)
            {
                change_status(current_process, FINISHED);
                remaining_process--;
                printf("%d, %s, process-name = %s, proc-remaining = %d\n", simulation_time, get_status_string(current_process), current_process->name, remaining_process);

                current_process->completion_time = simulation_time; //  time of completion for the process
                current_process->turnaround_time = simulation_time - current_process->arrival_time;
                current_process->time_overhead = (current_process->turnaround_time * 1.0) / current_process->service_time; // multiply with 1.0 to convert to float/double

                // no need to enqueue as process is finished
                // free_process(current_process);
                current_process = NULL;

                if (remaining_process == 0 && isEmpty(queue))
                {
                    should_loop = FALSE; // if we use a break, we can skip the should_loop variable
                    break;
                } // else loop continues
            }
            else
            {
                // requires more time

                if (!isEmpty(queue))
                {
                    change_status(current_process, READY);
                    enqueue(queue, current_process);
                    current_process = NULL;
                } // else loop keeps running, status is not changed
            }
        }

        // step 3: determine the process that runs in this cycle

        if (current_process == NULL && !isEmpty(queue))
        {
            current_process = dequeue(queue);
            change_status(current_process, RUNNING);

            // if (previous_process != current_process)
            printf("%d, %s, process-name = %s, remaining-time = %d\n", simulation_time, get_status_string(current_process), current_process->name, current_process->remaining_time);
        }

        if (current_process != NULL && current_process->remaining_time > 0)
        {
            (current_process->remaining_time -= quantum);
        }

        // update simulation time
        simulation_time += quantum;
    }

    *makespan = simulation_time;
}

void print_statistics(process_t **processes, int num_process, int makespan)
{
    printf("Turnaround time %d\n", get_average_turnaround(processes, num_process));

    printf("Time overhead %.2lf %.2lf\n", get_max_time_overhead(processes, num_process), get_average_time_overhead(processes, num_process));

    printf("Makespan %d\n", makespan);
}

int get_average_turnaround(process_t **processes, int num_process)
{

    int total = 0;

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
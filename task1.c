#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "task1.h"
#include "process.h"
#include "queue.h"

void round_robin(process_t **processes, queue_t *queue, int num_process, int quantum, int *makespan)
{
    int simulation_time = 0;
    int submitted_process = 0;
    int remaining_process = num_process;
    int ready_process_remaining = 0;
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
                ready_process_remaining = get_queue_length(queue);
                printf("%d,%s,process-name=%s,proc-remaining=%d\n", simulation_time, get_status_string(current_process), current_process->name, ready_process_remaining);

                current_process->completion_time = simulation_time; //  time of completion for the process
                current_process->turnaround_time = simulation_time - current_process->arrival_time;
                current_process->time_overhead = (current_process->turnaround_time * 1.0) / current_process->service_time; // multiply with 1.0 to convert to float/double

                // no need to enqueue as process is finished

                current_process = NULL;

                if (remaining_process == 0 && is_empty(queue))
                {
                    should_loop = FALSE; // if we use a break, we can skip the should_loop variable
                    break;
                } // else loop continues
            }
            else
            {
                // requires more time

                if (!is_empty(queue))
                {
                    change_status(current_process, READY);
                    enqueue(queue, current_process);
                    current_process = NULL;
                } // else loop keeps running, status is not changed
            }
        }

        // step 3: determine the process that runs in this cycle

        if (current_process == NULL && !is_empty(queue))
        {
            current_process = dequeue(queue);
            change_status(current_process, RUNNING);

            // if (previous_process != current_process)
            printf("%d,%s,process-name=%s,remaining-time=%d\n", simulation_time, get_status_string(current_process), current_process->name, current_process->remaining_time);
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
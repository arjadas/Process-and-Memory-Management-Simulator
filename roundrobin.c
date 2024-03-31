#include "roundrobin.h"

void roundRobin(process_t **processes, queue_t *queue, int num_process, int quantum)
{
    int simulation_time = 0;
    int submitted_process = 0;
    int remaining_process = num_process;
    int should_loop = TRUE;
    int process_remaining_time = 0;
    process_t *current_process = NULL;

    while (should_loop == TRUE)
    {

        // checking whether process is submitted to CPU
        while (submitted_process < num_process && processes[submitted_process]->arrival <= simulation_time)
        {
            change_status(processes[submitted_process], READY);
            enqueue(queue, processes[submitted_process]);
            submitted_process++;
        }

        // a cycle of looping
        if (!isEmpty(queue))
        {
            current_process = dequeue(queue);
            change_status(current_process, RUNNING);
            process_remaining_time = current_process->arrival + current_process->service_time - simulation_time;
            printf("%d, %s, process-name = %s, remaining-time = %d", simulation_time, current_process->status, current_process->name, process_remaining_time);
        }

        // update simulation time
        simulation_time += quantum;
        process_remaining_time = current_process->arrival + current_process->service_time - simulation_time; // assuming current_process != NULL

        if (process_remaining_time <= 0)
        {
            change_status(current_process, FINISHED);
            remaining_process--;
            printf("%d, %s, process-name = %s, proc-remaining = %d", simulation_time, current_process->status, current_process->name, process_remaining);

            // no need to enqueue as process is finished

            if (remaining_process == 0 && isEmpty(queue))
            {
                should_loop = FALSE;
            } // else loop continues
        }
        else
        {

            // requires more time

            if (!isEmpty(queue))
            {
                change_status(current_process, READY);
                enqueue(queue, current_process);
            } // else loop keeps running, status is not changed
        }
    }
}
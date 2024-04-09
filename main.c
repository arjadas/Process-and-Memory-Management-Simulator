#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"
#include "queue.h"
#include "roundrobin.h"

int main(int argc, char const *argv[])
{
    /*
        main function
    */

    int num_processes = 0;
    int makespan = 0;

    read_t *inputs = process_arguments(argc, argv);
    process_t **processes = read_processes(inputs, &num_processes);
    print_processes(processes, num_processes);

    queue_t *queue = createQueue();

    // task 1
    round_robin(processes, queue, num_processes, inputs->quantum, &makespan);
    print_statistics(processes, num_processes, makespan);

    free(queue);

    return 0;
}

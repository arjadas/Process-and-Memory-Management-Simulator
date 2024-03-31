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
    read_t *inputs = process_arguments(argc, argv);
    process_t **processes = read_processes(inputs, &num_processes);
    print_processes(processes, num_processes);

    queue_t *queue = createQueue();
    printf("%S", "entering");
    roundRobin(processes, queue, num_processes, 1);
    free(queue);

    return 0;
}

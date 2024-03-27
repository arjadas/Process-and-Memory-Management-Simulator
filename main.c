#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read.h"
#include "process.h"


int main(int argc, char const *argv[])
{
    /*
        main function
    */
    read_t *inputs = process_arguments(argc, argv);
    read_processes(inputs);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "memory.h"
#include "process.h"
#include "bitmap.h"

// we're cheating by not using actual bits

bitmap_t make_bitmap()
{
    bitmap_t bitmap = (int *)malloc(sizeof(int)*2048);
    for (int i = 0; i < 2048; i++)
    {
        bitmap[i] = 0;
    }
    return bitmap;
}

int allocate_memory(bitmap_t bitmap, process_t *process)
{
    /*
        in order to allocate memory we need to iterate through 
        bits to find a chunk of free memory of num_bytes
        then need to return the allocation
        an implementation the FIRST-FIT algorithm
    */
    // search for block
    int start = -1, count = 0;
    int num_bytes = process->memory_KB;
    

    for (int i = 0; i < 2048; i++)
    {
        if ((bitmap[i] == 0) && (count == 0))
        {
            start = i;
            count += 1;
        }
        else if ((bitmap[i]) == 0 && (count > 0))
        {
            count += 1;
            if ((bitmap[i] == 0) && (count == num_bytes)) // found a bit enough block
            {
                process->allocation->start = start;
                process->allocation->end = i;
                process->allocation->quantity = process->memory_KB;
                
                // allocate the memory
                for (int j = start; j <= i; j++)
                {
                    bitmap[j] = 1;
                }
                
                return 1;
            }

        }
        else if (bitmap[i] == 1)
        {
            start = -1, count = 0;
        }
    }
    // allocation failed
    return -1;
}

// deallocate memory
int deallocate_bitmap(bitmap_t bitmap, int start, int end)
{
    /*
        free memory
    */
    // need to find memory to free; end is inclusive
    for (int i = start; i <= end; i++)
    {
        bitmap[i] = 0;
    }
    return 0;
}

// destroy map not necessary as was statically allocated.
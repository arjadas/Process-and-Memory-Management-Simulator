#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "memory.h"
#include "process.h"
#include "bitmap.h"

// we're cheating by not using actual bits

bitmap_t *make_bitmap(int size)
{
    bitmap_t *bitmap = (bitmap_t *)malloc(sizeof(bitmap_t));
    bitmap->bitmap = (int *)malloc(sizeof(int) * size);
    bitmap->size = size;
    for (int i = 0; i < size; i++)
    {
        bitmap->bitmap[i] = 0;
    }
    bitmap->allocated = 0;
    return bitmap;
}

int allocate_memory(bitmap_t *bitmap, process_t *process)
{
    /*
        in order to allocate memory we need to iterate through
        bits to find a chunk of free memory of num_bytes
        then need to return the allocation
        an implementation of the FIRST-FIT algorithm
    */
    // search for block
    int start = -1, count = 0;
    int num_bytes = process->memory_KB;

    for (int i = 0; i < bitmap->size; i++)
    {
        if (((bitmap->bitmap)[i] == 0) && (count == 0))
        {
            start = i;
            count += 1;
        }
        else if (((bitmap->bitmap)[i]) == 0 && (count > 0))
        {
            count += 1;
            if (((bitmap->bitmap)[i] == 0) && (count == num_bytes)) // found a bit enough block
            {
                process->allocation->start = start;
                process->allocation->end = i;
                process->allocation->quantity = process->memory_KB;

                // allocate the memory
                for (int j = start; j <= i; j++)
                {
                    (bitmap->bitmap)[j] = 1;
                }
                // allocation was successful
                bitmap->allocated += process->memory_KB;
                return 1;
            }
        }
        else if ((bitmap->bitmap)[i] == 1)
        {
            start = -1, count = 0;
        }
    }
    // allocation failed
    return INVALID;
}

// deallocate memory
int deallocate_bitmap(bitmap_t *bitmap, memory_t *memory)
{
    /*
        free memory
    */
    // need to find memory to free; end is inclusive
    for (int i = memory->start; i <= memory->end; i++)
    {
        (bitmap->bitmap)[i] = 0;
    }
    bitmap->allocated -= memory->quantity;
    return 0;
}

// destroy map not necessary as was statically allocated.
void destroy_map(bitmap_t *bitmap)
{
    free(bitmap->bitmap);
    free(bitmap);
}
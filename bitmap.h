#ifndef BITMAP_H
#define BITMAP_H
#include "process.h"

typedef struct {
    int* bitmap;
    int allocated;  
    int size;
} bitmap_t;

bitmap_t *make_bitmap(int size);
int allocate_memory(bitmap_t *bitmap, process_t *process);
int deallocate_bitmap(bitmap_t *bitmap, memory_t *memory);
void destroy_map(bitmap_t *bitmap);

#endif
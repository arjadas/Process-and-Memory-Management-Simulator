#ifndef BITMAP_H
#define BITMAP_H
#include "process.h"

typedef int* bitmap_t;

bitmap_t make_bitmap();
int allocate_memory(bitmap_t bitmap, process_t *process);
int deallocate_bitmap(bitmap_t bitmap, int start, int end);

#endif
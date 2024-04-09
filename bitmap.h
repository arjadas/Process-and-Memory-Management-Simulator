#ifndef BITMAP_H
#define BITMAP_H
#include "process.h"

int *make_bitmap();
int allocate_memory(int *bitmap, process_t *process);
int deallocate_bitmap(int *bitmap, int start, int end);

#endif
#ifndef MEMORY_H
#define MEMORY_H

typedef struct memory memory_t;
struct memory
{
    int start;    // start of the allocation in the bitmap
    int end;      // end of the allocation in the bitmap (inclusive)
    int quantity; // number of units allocated
};

#endif
#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"

// queue data structure
typedef struct node node_t;

struct node
{
    process_t *data;
    node_t *next;
};

// queue data structure
typedef struct
{
    node_t *head;
    node_t *tail;
    int length;
} queue_t;

// prototypes
queue_t *create_queue();
int is_empty(queue_t *queue);
void enqueue(queue_t *queue, process_t *process);
process_t *dequeue(queue_t *queue);
void empty_queue(queue_t *queue);

#endif
#ifndef QUEUE_H
#define QUEUE_H

// node to store the process data

typedef struct node;

typedef struct
{
    process_t data;
    struct node *next;
} node_t;

// queue data structure

typedef struct
{
    node_t *head;
    node_t *tail;
} queue_t;

// prototypes

queue_t *createQueue();
int isEmpty(queue_t *queue);
void enqueue(queue_t *queue, process_t *process);

#endif
#include "queue.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

// function to create a new queue
queue_t *createQueue()
{

    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));

    if (queue == NULL)
    {
        printf("%s", "Memory Allocation for queue failed");
        exit(EXIT_FAILURE);
    }

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

// function to check whether the queue is empty
int isEmpty(queue_t *queue)
{

    return (queue->head == NULL);
}

// function to add a process to the queue
void enqueue(queue_t *queue, process_t *process)
{

    // creating a new node to store the new process
    node_t *newNode = (node_t *)malloc(sizeof(node_t));

    if (newNode == NULL)
    {
        printf("%s", "Memory Allocation for new node failed");
        exit(EXIT_FAILURE);
    }

    newNode->data = process;
    newNode->next = NULL;

    if (isEmpty(queue))
    {
        queue->head = newNode;
        queue->tail = newNode;
    }
    else
    {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}
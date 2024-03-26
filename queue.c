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
        printf("%s", "Memory Allocation for queue failed"); // is it a proper response?
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
        printf("%s", "Memory Allocation for new node failed"); // is it a proper response?
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

// function to remove an element from the queue
process_t *dequeue(queue_t *queue)
{

    if (isEmpty(queue))
    {
        printf("%s", "Queue is empty, nothing to dequeue"); // is it a proper response?
    }

    node_t *item_node = queue->head;

    queue->head = queue->head->next;

    if (queue->head == NULL)
    { // there was only one element in the queue, so the tail needs to point to null too
        queue->tail = NULL;
    }

    item_node->next = NULL;

    process_t *data = item_node->data;

    // deleting the content and freeing the node
    item_node->data = NULL;
    item_node->next = NULL;
    free(item_node);

    return data;
}

// function to destroy the queue
void emptyQueue(queue_t *queue)
{

    if (isEmpty(queue))
    {
        printf("%s", "Queue already empty"); // is it a proper response?
    }

    while (!isEmpty(queue))
    {
        dequeue(queue);
        // node is already getting free'd while dequeuing
    }

    // freeing the queue
    free(queue);
}

#include "queue.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// function to create a new queue
queue_t *create_queue()
{

    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    assert(queue);

    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;

    return queue;
}

// function to check whether the queue is empty
int is_empty(queue_t *queue)
{
    return (queue->head == NULL);
}

void enqueue(queue_t *queue, process_t *process)
{
    /*
        function to add an item to the queue
        inputs: queue and a pointer to a process
    */

    // creating a new node to store the new process
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    assert(newNode);

    newNode->data = process;
    newNode->next = NULL;

    if (is_empty(queue))
    {
        queue->head = newNode;
        queue->tail = newNode;
    }
    else
    {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->length += 1;
}

process_t *dequeue(queue_t *queue)
{
    /*
        function to remove an element from the queue
        inputs: a pointer to a queue
        return: returns a pointer to the process removed from the queue
    */

    // perhaps we should return NULL instead?
    if (is_empty(queue))
    {
        printf("%s", "Queue is empty, nothing to dequeue"); // is it a proper response?
        return NULL;
    }

    node_t *item_node = queue->head;

    queue->head = queue->head->next;

    if (queue->head == NULL)
    {
        // there was only one element in the queue, so the tail needs to point to null too
        queue->tail = NULL;
    }
    queue->length -= 1;
    item_node->next = NULL;

    process_t *data = item_node->data;

    // deleting the content and freeing the node
    item_node->data = NULL;
    item_node->next = NULL;
    free(item_node);

    return data;
}

void empty_queue(queue_t *queue)
{
    /*
        function to destroy the queue
    */

    if (is_empty(queue))
    {
        printf("%s", "Queue already empty"); // is it a proper response?
    }

    while (!is_empty(queue))
    {
        // node is already getting free'd while dequeuing
        dequeue(queue);
    }

    // freeing the queue
    free(queue);
}

int get_queue_length(queue_t *queue)
{
    /*
        function to return length of the queue
    */

    /*
     int len = 0;

     if (is_empty(queue)) {
         return len;
     } else {
         return queue->length;
     }
     */

    return queue->length;
}
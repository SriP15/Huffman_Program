#include <stdio.h>
#include "pq.h"
#include <stdlib.h>
#include "node.h"
#include <stdbool.h>
#include <inttypes.h>

//The struct for a Priority Queue.
//Contains a uint32_t head value, and uint32_t tail value.
//Contains a uint32_t size value, and uint32_t capacity value.
//Also contains an array of node pointers called items.
struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    uint32_t size;
    Node **items;
};

//Dynamically allocates memory for a Priority Queue, and initializes it.
//Returns a Priority Queue pointer.
//
//capacity: a uint32_t value that stores the capacity of the Priority Queue.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(capacity * sizeof(PriorityQueue));
    if (pq) {
        pq->head = 0;
        pq->tail = 0;
        pq->size = 0;
        pq->capacity = capacity;
        Node **pq_items = (Node **) calloc(capacity, sizeof(Node *));
        /*for (uint32_t i = 0; i < capacity; i += 1) {
            pq_items[i] = (Node *) calloc(1, sizeof(Node));
        }*/
        pq->items = pq_items;
        return pq;
    }
    return ((PriorityQueue *) NULL);
}

//Frees memory used by a Priority Queue.
//Returns nothing (void).
//
//q: a pointer to a pointer of a Priority Queue.
void pq_delete(PriorityQueue **q) {
    /*for (uint32_t i = 0; i < (*q)->capacity; i += 1) {
        node_delete(&(*q)->items[i]);//free(((*q)->items[i]));
    }*/
    free((*q)->items);
    //(*q)->items = NULL;
    //free(*q);
    //*q = NULL;
}

//Checks to see if a Priority Queue is empty.
//Returns a boolean. If the Priority Queue is empty, returns true. If the
//Priority queue is not empty returns false.
//
//q: a pointer to a Priority Queue.
bool pq_empty(PriorityQueue *q) {
    if (q->head == q->tail) {
        return true;
    } else {
        return false;
    }
}

//Checks to see if a Priority Queue is full.
//Returns a boolean. If the Priority Queue is full: returns true. If the
//Priority Queue is not full returns false.
//
//q: a pointer to a Priority Queue.
bool pq_full(PriorityQueue *q) {
    if (q->head == (q->capacity)) {
        return true;
    } else {
        return false;
    }
}

//Finds the size of a Priority Queue.
//Returns a uint32_t that indicates the size of a Priority Queue.
//
//q: a pointer to a prioity queue.
uint32_t pq_size(PriorityQueue *q) {
    return (q->size);
}

//Enqueues a node into the correct position of a Priority Queue based on its frequency.
//The lower the frequency, the higher its priority.
//Returns a boolean indicating if the node was enqueued. Returns false if the Priority Queue is full, and
//so the node was not enqueued.
//
//q: a pointer to a Priority Queue.
//n: a pointer to a node that is to be enqueued.
bool enqueue(PriorityQueue *q, Node *n) {
    uint32_t j = pq_size(q) + q->tail;
    uint32_t k = pq_size(q);
    ;
    Node *temp = n;
    if (!n) {
        return false;
    }

    if (pq_full(q)) {
        return false;
    }

    if (pq_empty(q)) {
        q->items[q->tail] = temp;
    } else {
        while ((k > 0) && ((temp->frequency) < (q->items[j - 1]->frequency))) {
            q->items[j] = q->items[j - 1];
            k -= 1;
            j -= 1;
        }
        q->items[j] = temp;
    }
    q->size += 1;
    q->head = (q->head + 1);
    return true;
}

//Dequeues a node from a Priority Queue, saving the value of the node in *n.
//Returns a bool that indicates if the node was dequeued successfully.
//Returns true if node was dequeued. Returns false if Priority Queue is empty, and
//so the node was not dequeued.
//
//q: a pointer to a Priority Queue.
//n: a pointer to a pointer of node that is to be dequeued.
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }

    *n = q->items[q->tail];
    q->tail = (q->tail + 1);
    q->size -= 1;
    return true;
}

//A debugger function that prints the values of a Priority Queue. The Priority Queue must already be created.
//Returns void.
//
//q: a pointer to a Priority Queue.
void pq_print(PriorityQueue *q) {

    for (uint32_t i = 0; i < q->capacity; i += 1) {
        printf("item[%" PRIu32 "] symbol is %" PRIu8 ", and frequency is %" PRIu64 ".\n", i,
            q->items[i]->symbol, q->items[i]->frequency);
    }
}

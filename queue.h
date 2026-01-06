#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "appointment.h"

/**
 * Queue Node Structure
 * Used for waiting list when doctors are fully booked
 */
typedef struct QueueNode {
    Appointment* appointment;  // Pointer to appointment in waiting list
    struct QueueNode* next;    // Pointer to next node in queue
} QueueNode;

/**
 * Queue Structure
 * FIFO (First In First Out) data structure
 * Used to manage waiting list for fully booked doctors
 */
typedef struct {
    QueueNode* front;  // Pointer to front of queue
    QueueNode* rear;   // Pointer to rear of queue
    int size;          // Current number of elements in queue
} Queue;

/**
 * Queue Operations
 * Time Complexity: O(1) for enqueue/dequeue/isEmpty
 */

/**
 * Initializes an empty queue
 * @param queue: Pointer to queue to initialize
 */
void queue_init(Queue* queue);

/**
 * Adds an appointment to the rear of queue (waiting list)
 * @param queue: Pointer to queue
 * @param appointment: Pointer to appointment to enqueue
 * @return: 1 on success, 0 on failure
 */
int queue_enqueue(Queue* queue, Appointment* appointment);

/**
 * Removes an appointment from the front of queue
 * @param queue: Pointer to queue
 * @return: Pointer to appointment, NULL if queue is empty
 */
Appointment* queue_dequeue(Queue* queue);

/**
 * Checks if queue is empty
 * @param queue: Pointer to queue
 * @return: 1 if empty, 0 otherwise
 */
int queue_is_empty(const Queue* queue);

/**
 * Returns the number of elements in queue
 * @param queue: Pointer to queue
 * @return: Size of queue
 */
int queue_size(const Queue* queue);

/**
 * Frees all memory allocated for the queue
 * @param queue: Pointer to queue to destroy
 */
void queue_destroy(Queue* queue);

#endif // QUEUE_H


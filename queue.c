/**
 * Queue Implementation
 * 
 * FIFO (First In First Out) data structure
 * Used for waiting list - manages appointments waiting for available slots
 * 
 * Time Complexity:
 * - enqueue: O(1)
 * - dequeue: O(1)
 * - isEmpty: O(1)
 * - size: O(1)
 */

#include "queue.h"

/**
 * Initializes an empty queue
 * Time Complexity: O(1)
 * 
 * @param queue: Pointer to queue to initialize
 */
void queue_init(Queue* queue) {
    if (queue == NULL) {
        return;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

/**
 * Adds an appointment to the rear of queue (waiting list)
 * Time Complexity: O(1) - constant time insertion at tail
 * 
 * @param queue: Pointer to queue
 * @param appointment: Pointer to appointment to enqueue
 * @return: 1 on success, 0 on failure
 */
int queue_enqueue(Queue* queue, Appointment* appointment) {
    if (queue == NULL || appointment == NULL) {
        return 0;
    }

    // Allocate memory for new queue node
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        return 0; // Memory allocation failed
    }

    // Initialize new node
    new_node->appointment = appointment;
    new_node->next = NULL; // New node is always at the rear

    // Update queue
    if (queue->rear == NULL) {
        // Queue is empty - new node becomes both front and rear
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        // Queue has elements - add to rear
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;
    return 1; // Success
}

/**
 * Removes an appointment from the front of queue
 * Time Complexity: O(1) - constant time removal from head
 * 
 * @param queue: Pointer to queue
 * @return: Pointer to appointment, NULL if queue is empty
 */
Appointment* queue_dequeue(Queue* queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL; // Queue is empty
    }

    // Get the front node
    QueueNode* node_to_remove = queue->front;
    Appointment* appointment = node_to_remove->appointment;

    // Update queue front
    queue->front = queue->front->next;
    queue->size--;

    // If queue becomes empty, update rear as well
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // Free the node (but not the appointment - caller owns it)
    free(node_to_remove);

    return appointment;
}

/**
 * Checks if queue is empty
 * Time Complexity: O(1)
 * 
 * @param queue: Pointer to queue
 * @return: 1 if empty, 0 otherwise
 */
int queue_is_empty(const Queue* queue) {
    if (queue == NULL) {
        return 1; // Consider NULL as empty
    }
    return (queue->front == NULL) ? 1 : 0;
}

/**
 * Returns the number of elements in queue
 * Time Complexity: O(1)
 * 
 * @param queue: Pointer to queue
 * @return: Size of queue
 */
int queue_size(const Queue* queue) {
    if (queue == NULL) {
        return 0;
    }
    return queue->size;
}

/**
 * Frees all memory allocated for the queue
 * Time Complexity: O(n) where n is the number of nodes
 * 
 * @param queue: Pointer to queue to destroy
 */
void queue_destroy(Queue* queue) {
    if (queue == NULL) {
        return;
    }

    // Dequeue all nodes until queue is empty
    while (!queue_is_empty(queue)) {
        QueueNode* node_to_remove = queue->front;
        queue->front = queue->front->next;
        
        // Note: We don't free appointments here - they are managed elsewhere
        free(node_to_remove);
    }

    queue->rear = NULL;
    queue->size = 0;
}


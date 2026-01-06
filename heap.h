#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include "appointment.h"

/**
 * Min-Heap Structure
 * Complete binary tree stored as array
 * Used to efficiently find earliest appointment
 * Parent is always <= children (min-heap property)
 */
typedef struct {
    Appointment** appointments;  // Array of appointment pointers
    int capacity;                // Maximum capacity of heap
    int size;                    // Current number of elements
} MinHeap;

/**
 * Min-Heap Operations
 * Time Complexity: O(log n) for insert/extract_min, O(1) for peek
 */

/**
 * Initializes an empty min-heap
 * @param heap: Pointer to heap to initialize
 * @param capacity: Maximum capacity of heap
 * @return: 1 on success, 0 on failure
 */
int heap_init(MinHeap* heap, int capacity);

/**
 * Inserts an appointment into the min-heap
 * Maintains min-heap property (earliest time at root)
 * @param heap: Pointer to heap
 * @param appointment: Pointer to appointment to insert
 * @return: 1 on success, 0 on failure
 */
int heap_insert(MinHeap* heap, Appointment* appointment);

/**
 * Extracts and removes the appointment with minimum start_time
 * @param heap: Pointer to heap
 * @return: Pointer to earliest appointment, NULL if heap is empty
 */
Appointment* heap_extract_min(MinHeap* heap);

/**
 * Returns the earliest appointment without removing it
 * @param heap: Pointer to heap
 * @return: Pointer to earliest appointment, NULL if heap is empty
 */
Appointment* heap_peek_min(const MinHeap* heap);

/**
 * Checks if heap is empty
 * @param heap: Pointer to heap
 * @return: 1 if empty, 0 otherwise
 */
int heap_is_empty(const MinHeap* heap);

/**
 * Returns the number of elements in heap
 * @param heap: Pointer to heap
 * @return: Size of heap
 */
int heap_size(const MinHeap* heap);

/**
 * Frees all memory allocated for the heap
 * @param heap: Pointer to heap to destroy
 */
void heap_destroy(MinHeap* heap);

/**
 * Internal helper function: Restores min-heap property from root downward
 * Time Complexity: O(log n)
 */
void heapify_down(MinHeap* heap, int index);

/**
 * Internal helper function: Restores min-heap property from leaf upward
 * Time Complexity: O(log n)
 */
void heapify_up(MinHeap* heap, int index);

#endif // HEAP_H


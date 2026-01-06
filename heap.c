/**
 * Min-Heap Implementation
 * 
 * Complete binary tree stored as array
 * Used to efficiently find earliest appointment
 * Parent is always <= children (min-heap property)
 * 
 * Time Complexity:
 * - insert: O(log n)
 * - extract_min: O(log n)
 * - peek_min: O(1)
 * - isEmpty: O(1)
 * - size: O(1)
 * 
 * Space Complexity: O(n) where n is capacity
 */

#include "heap.h"

/**
 * Initializes an empty min-heap
 * Time Complexity: O(1)
 * 
 * @param heap: Pointer to heap to initialize
 * @param capacity: Maximum capacity of heap
 * @return: 1 on success, 0 on failure
 */
int heap_init(MinHeap* heap, int capacity) {
    if (heap == NULL || capacity <= 0) {
        return 0;
    }

    // Allocate memory for appointment pointer array
    heap->appointments = (Appointment**)malloc(capacity * sizeof(Appointment*));
    if (heap->appointments == NULL) {
        return 0; // Memory allocation failed
    }

    heap->capacity = capacity;
    heap->size = 0;

    return 1; // Success
}

/**
 * Internal helper: Restores min-heap property from root downward
 * Used after extract_min to maintain heap property
 * Time Complexity: O(log n) - height of tree
 * 
 * @param heap: Pointer to heap
 * @param index: Index to start heapifying from
 */
void heapify_down(MinHeap* heap, int index) {
    if (heap == NULL || index < 0 || index >= heap->size) {
        return;
    }

    int smallest = index;        // Assume current node is smallest
    int left = 2 * index + 1;    // Left child index
    int right = 2 * index + 2;   // Right child index

    // Compare with left child
    if (left < heap->size && 
        heap->appointments[left]->start_time < heap->appointments[smallest]->start_time) {
        smallest = left;
    }

    // Compare with right child
    if (right < heap->size && 
        heap->appointments[right]->start_time < heap->appointments[smallest]->start_time) {
        smallest = right;
    }

    // If smallest is not the current node, swap and recurse
    if (smallest != index) {
        // Swap appointments
        Appointment* temp = heap->appointments[index];
        heap->appointments[index] = heap->appointments[smallest];
        heap->appointments[smallest] = temp;

        // Recursively heapify the affected subtree
        heapify_down(heap, smallest);
    }
}

/**
 * Internal helper: Restores min-heap property from leaf upward
 * Used after insert to maintain heap property
 * Time Complexity: O(log n) - height of tree
 * 
 * @param heap: Pointer to heap
 * @param index: Index to start heapifying from
 */
void heapify_up(MinHeap* heap, int index) {
    if (heap == NULL || index <= 0 || index >= heap->size) {
        return;
    }

    int parent = (index - 1) / 2; // Parent index

    // If child is smaller than parent, swap and recurse
    if (heap->appointments[index]->start_time < heap->appointments[parent]->start_time) {
        // Swap appointments
        Appointment* temp = heap->appointments[index];
        heap->appointments[index] = heap->appointments[parent];
        heap->appointments[parent] = temp;

        // Recursively heapify upward
        heapify_up(heap, parent);
    }
}

/**
 * Inserts an appointment into the min-heap
 * Maintains min-heap property (earliest time at root)
 * Time Complexity: O(log n) - height of tree for heapify_up
 * 
 * @param heap: Pointer to heap
 * @param appointment: Pointer to appointment to insert
 * @return: 1 on success, 0 on failure
 */
int heap_insert(MinHeap* heap, Appointment* appointment) {
    if (heap == NULL || appointment == NULL) {
        return 0;
    }

    // Check if heap is full
    if (heap->size >= heap->capacity) {
        return 0; // Heap is full
    }

    // Add appointment at the end
    heap->appointments[heap->size] = appointment;
    heap->size++;

    // Restore min-heap property by moving new element up
    heapify_up(heap, heap->size - 1);

    return 1; // Success
}

/**
 * Extracts and removes the appointment with minimum start_time
 * Time Complexity: O(log n) - height of tree for heapify_down
 * 
 * @param heap: Pointer to heap
 * @return: Pointer to earliest appointment, NULL if heap is empty
 */
Appointment* heap_extract_min(MinHeap* heap) {
    if (heap == NULL || heap->size == 0) {
        return NULL; // Heap is empty
    }

    // Root contains the minimum element
    Appointment* min_appointment = heap->appointments[0];

    // Move last element to root
    heap->appointments[0] = heap->appointments[heap->size - 1];
    heap->size--;

    // Restore min-heap property by moving root down
    if (heap->size > 0) {
        heapify_down(heap, 0);
    }

    return min_appointment;
}

/**
 * Returns the earliest appointment without removing it
 * Time Complexity: O(1) - direct access to root
 * 
 * @param heap: Pointer to heap
 * @return: Pointer to earliest appointment, NULL if heap is empty
 */
Appointment* heap_peek_min(const MinHeap* heap) {
    if (heap == NULL || heap->size == 0) {
        return NULL; // Heap is empty
    }

    return heap->appointments[0]; // Root is always minimum
}

/**
 * Checks if heap is empty
 * Time Complexity: O(1)
 * 
 * @param heap: Pointer to heap
 * @return: 1 if empty, 0 otherwise
 */
int heap_is_empty(const MinHeap* heap) {
    if (heap == NULL) {
        return 1; // Consider NULL as empty
    }
    return (heap->size == 0) ? 1 : 0;
}

/**
 * Returns the number of elements in heap
 * Time Complexity: O(1)
 * 
 * @param heap: Pointer to heap
 * @return: Size of heap
 */
int heap_size(const MinHeap* heap) {
    if (heap == NULL) {
        return 0;
    }
    return heap->size;
}

/**
 * Frees all memory allocated for the heap
 * Time Complexity: O(1)
 * 
 * Note: Does not free appointments - they are managed elsewhere
 * 
 * @param heap: Pointer to heap to destroy
 */
void heap_destroy(MinHeap* heap) {
    if (heap == NULL) {
        return;
    }

    // Free the appointments array
    if (heap->appointments != NULL) {
        free(heap->appointments);
        heap->appointments = NULL;
    }

    heap->capacity = 0;
    heap->size = 0;
}


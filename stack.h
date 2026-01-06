#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "appointment.h"

/**
 * Stack Node Structure
 * Used for undo system (cancelled appointment recovery)
 */
typedef struct StackNode {
    Appointment* appointment;  // Pointer to cancelled appointment
    struct StackNode* next;    // Pointer to next node in stack
} StackNode;

/**
 * Stack Structure
 * LIFO (Last In First Out) data structure
 * Used to store cancelled appointments for undo functionality
 */
typedef struct {
    StackNode* top;  // Pointer to top of stack
    int size;        // Current number of elements in stack
} Stack;

/**
 * Stack Operations
 * Time Complexity: O(1) for push/pop/isEmpty
 */

/**
 * Initializes an empty stack
 * @param stack: Pointer to stack to initialize
 */
void stack_init(Stack* stack);

/**
 * Pushes an appointment onto the stack (for undo)
 * @param stack: Pointer to stack
 * @param appointment: Pointer to appointment to push
 * @return: 1 on success, 0 on failure
 */
int stack_push(Stack* stack, Appointment* appointment);

/**
 * Pops an appointment from the stack (undo operation)
 * @param stack: Pointer to stack
 * @return: Pointer to appointment, NULL if stack is empty
 */
Appointment* stack_pop(Stack* stack);

/**
 * Checks if stack is empty
 * @param stack: Pointer to stack
 * @return: 1 if empty, 0 otherwise
 */
int stack_is_empty(const Stack* stack);

/**
 * Returns the number of elements in stack
 * @param stack: Pointer to stack
 * @return: Size of stack
 */
int stack_size(const Stack* stack);

/**
 * Frees all memory allocated for the stack
 * @param stack: Pointer to stack to destroy
 */
void stack_destroy(Stack* stack);

#endif // STACK_H


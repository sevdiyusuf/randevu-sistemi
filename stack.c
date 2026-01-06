/**
 * Stack Implementation
 * 
 * LIFO (Last In First Out) data structure
 * Used for undo system - stores cancelled appointments
 * 
 * Time Complexity:
 * - push: O(1)
 * - pop: O(1)
 * - isEmpty: O(1)
 * - size: O(1)
 */

#include "stack.h"

/**
 * Initializes an empty stack
 * Time Complexity: O(1)
 * 
 * @param stack: Pointer to stack to initialize
 */
void stack_init(Stack* stack) {
    if (stack == NULL) {
        return;
    }
    stack->top = NULL;
    stack->size = 0;
}

/**
 * Pushes an appointment onto the stack (for undo)
 * Time Complexity: O(1) - constant time insertion at head
 * 
 * @param stack: Pointer to stack
 * @param appointment: Pointer to appointment to push
 * @return: 1 on success, 0 on failure
 */
int stack_push(Stack* stack, Appointment* appointment) {
    if (stack == NULL || appointment == NULL) {
        return 0;
    }

    // Allocate memory for new stack node
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node == NULL) {
        return 0; // Memory allocation failed
    }

    // Initialize new node
    new_node->appointment = appointment;
    new_node->next = stack->top; // Link to previous top

    // Update stack
    stack->top = new_node;
    stack->size++;

    return 1; // Success
}

/**
 * Pops an appointment from the stack (undo operation)
 * Time Complexity: O(1) - constant time removal from head
 * 
 * @param stack: Pointer to stack
 * @return: Pointer to appointment, NULL if stack is empty
 */
Appointment* stack_pop(Stack* stack) {
    if (stack == NULL || stack->top == NULL) {
        return NULL; // Stack is empty
    }

    // Get the top node
    StackNode* node_to_remove = stack->top;
    Appointment* appointment = node_to_remove->appointment;

    // Update stack top
    stack->top = stack->top->next;
    stack->size--;

    // Free the node (but not the appointment - caller owns it)
    free(node_to_remove);

    return appointment;
}

/**
 * Checks if stack is empty
 * Time Complexity: O(1)
 * 
 * @param stack: Pointer to stack
 * @return: 1 if empty, 0 otherwise
 */
int stack_is_empty(const Stack* stack) {
    if (stack == NULL) {
        return 1; // Consider NULL as empty
    }
    return (stack->top == NULL) ? 1 : 0;
}

/**
 * Returns the number of elements in stack
 * Time Complexity: O(1)
 * 
 * @param stack: Pointer to stack
 * @return: Size of stack
 */
int stack_size(const Stack* stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}

/**
 * Frees all memory allocated for the stack
 * Time Complexity: O(n) where n is the number of nodes
 * 
 * @param stack: Pointer to stack to destroy
 */
void stack_destroy(Stack* stack) {
    if (stack == NULL) {
        return;
    }

    // Pop all nodes until stack is empty
    while (!stack_is_empty(stack)) {
        StackNode* node_to_remove = stack->top;
        stack->top = stack->top->next;
        
        // Note: We don't free appointments here - they are managed elsewhere
        free(node_to_remove);
    }

    stack->size = 0;
}


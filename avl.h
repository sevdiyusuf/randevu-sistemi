#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include "appointment.h"

/**
 * AVL Tree Node Structure
 * Self-balancing binary search tree node
 * Key: appointment.start_time (used for efficient time-based queries)
 */
typedef struct AVLNode {
    Appointment* appointment;      // Pointer to appointment data
    struct AVLNode* left;          // Left child
    struct AVLNode* right;         // Right child
    int height;                    // Height of subtree rooted at this node
} AVLNode;

/**
 * AVL Tree Structure
 * Self-balancing binary search tree
 * Used for efficient appointment storage and conflict detection
 */
typedef struct {
    AVLNode* root;  // Root node of AVL tree
    int size;       // Number of appointments in tree
} AVLTree;

/**
 * AVL Tree Operations
 * Time Complexity: O(log n) for insert/search/delete due to balancing
 */

/**
 * Initializes an empty AVL tree
 * @param tree: Pointer to tree to initialize
 */
void avl_init(AVLTree* tree);

/**
 * Inserts an appointment into AVL tree
 * Automatically balances tree and checks for time conflicts
 * @param tree: Pointer to AVL tree
 * @param appointment: Pointer to appointment to insert
 * @param doctor_id: Doctor ID for conflict checking
 * @return: 1 on success, 0 if conflict detected or insertion failed
 */
int avl_insert(AVLTree* tree, Appointment* appointment, int doctor_id);

/**
 * Searches for appointments in a given time range
 * @param tree: Pointer to AVL tree
 * @param doctor_id: Doctor ID to search for
 * @param start_time: Start of time range
 * @param end_time: End of time range
 * @param results: Array to store found appointments (must be pre-allocated)
 * @param max_results: Maximum number of results
 * @return: Number of appointments found
 */
int avl_search_range(AVLTree* tree, int doctor_id, int start_time, 
                     int end_time, Appointment** results, int max_results);

/**
 * Checks if an appointment time conflicts with existing appointments
 * for the same doctor
 * @param tree: Pointer to AVL tree
 * @param doctor_id: Doctor ID to check
 * @param start_time: Proposed start time
 * @param end_time: Proposed end time
 * @return: 1 if conflict exists, 0 otherwise
 */
int avl_has_conflict(AVLTree* tree, int doctor_id, int start_time, int end_time);

/**
 * Deletes an appointment from AVL tree by appointment_id
 * Automatically balances tree after deletion
 * @param tree: Pointer to AVL tree
 * @param appointment_id: ID of appointment to delete
 * @return: Pointer to deleted appointment, NULL if not found
 */
Appointment* avl_delete(AVLTree* tree, int appointment_id);

/**
 * Searches for an appointment by appointment_id
 * @param tree: Pointer to AVL tree
 * @param appointment_id: ID to search for
 * @return: Pointer to appointment if found, NULL otherwise
 */
Appointment* avl_search_by_id(AVLTree* tree, int appointment_id);

/**
 * Returns the number of appointments in tree
 * @param tree: Pointer to AVL tree
 * @return: Size of tree
 */
int avl_size(const AVLTree* tree);

/**
 * Checks if tree is empty
 * @param tree: Pointer to AVL tree
 * @return: 1 if empty, 0 otherwise
 */
int avl_is_empty(const AVLTree* tree);

/**
 * Frees all memory allocated for the AVL tree
 * @param tree: Pointer to tree to destroy
 */
void avl_destroy(AVLTree* tree);

/**
 * Internal helper functions for AVL balancing
 */

/**
 * Performs right rotation (for left-heavy tree)
 * Time Complexity: O(1)
 */
AVLNode* avl_rotate_right(AVLNode* node);

/**
 * Performs left rotation (for right-heavy tree)
 * Time Complexity: O(1)
 */
AVLNode* avl_rotate_left(AVLNode* node);

/**
 * Calculates height of a node
 * Time Complexity: O(1)
 */
int avl_get_height(AVLNode* node);

/**
 * Calculates balance factor of a node
 * Time Complexity: O(1)
 */
int avl_get_balance(AVLNode* node);

/**
 * Updates height of a node based on children
 * Time Complexity: O(1)
 */
void avl_update_height(AVLNode* node);

#endif // AVL_H


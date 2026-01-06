/**
 * AVL Tree Implementation
 * 
 * Self-balancing binary search tree
 * Used for efficient appointment storage and conflict detection
 * 
 * Key: appointment.start_time (for efficient time-based queries)
 * 
 * Why AVL Tree over Regular BST?
 * - Regular BST can degrade to O(n) in worst case (e.g., sorted input)
 * - AVL guarantees O(log n) performance through automatic balancing
 * - Critical for maintaining performance as appointment count grows
 * - Self-balancing ensures tree height is always O(log n)
 * 
 * Time Complexity:
 * - insert: O(log n) - tree height + conflict check
 * - delete: O(log n) - tree height
 * - search: O(log n) - tree height
 * - rotations: O(1) - constant time operations
 */

#include "avl.h"

/**
 * Calculates height of a node
 * Returns -1 for NULL nodes (empty subtree)
 * Time Complexity: O(1)
 * 
 * @param node: Pointer to AVL node
 * @return: Height of node, -1 if NULL
 */
int avl_get_height(AVLNode* node) {
    if (node == NULL) {
        return -1; // Empty subtree has height -1
    }
    return node->height;
}

/**
 * Updates height of a node based on children's heights
 * Time Complexity: O(1)
 * 
 * @param node: Pointer to AVL node
 */
void avl_update_height(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    int left_height = avl_get_height(node->left);
    int right_height = avl_get_height(node->right);

    // Height is 1 + max of left and right subtree heights
    node->height = 1 + ((left_height > right_height) ? left_height : right_height);
}

/**
 * Calculates balance factor of a node
 * Balance factor = height(left) - height(right)
 * Valid range: -1, 0, 1 (balanced)
 * Time Complexity: O(1)
 * 
 * @param node: Pointer to AVL node
 * @return: Balance factor
 */
int avl_get_balance(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return avl_get_height(node->left) - avl_get_height(node->right);
}

/**
 * Performs right rotation (for left-heavy tree)
 * 
 *     y                x
 *    / \              / \
 *   x   C    -->     A   y
 *  / \                   / \
 * A   B                 B   C
 * 
 * Time Complexity: O(1)
 * 
 * @param y: Pointer to unbalanced node
 * @return: Pointer to new root after rotation
 */
AVLNode* avl_rotate_right(AVLNode* y) {
    if (y == NULL || y->left == NULL) {
        return y; // Cannot rotate
    }

    AVLNode* x = y->left;
    AVLNode* B = x->right;

    // Perform rotation
    x->right = y;
    y->left = B;

    // Update heights (must update y first, then x)
    avl_update_height(y);
    avl_update_height(x);

    return x; // x is new root
}

/**
 * Performs left rotation (for right-heavy tree)
 * 
 *   x                    y
 *  / \                  / \
 * A   y      -->       x   C
 *    / \              / \
 *   B   C            A   B
 * 
 * Time Complexity: O(1)
 * 
 * @param x: Pointer to unbalanced node
 * @return: Pointer to new root after rotation
 */
AVLNode* avl_rotate_left(AVLNode* x) {
    if (x == NULL || x->right == NULL) {
        return x; // Cannot rotate
    }

    AVLNode* y = x->right;
    AVLNode* B = y->left;

    // Perform rotation
    y->left = x;
    x->right = B;

    // Update heights (must update x first, then y)
    avl_update_height(x);
    avl_update_height(y);

    return y; // y is new root
}

/**
 * Initializes an empty AVL tree
 * Time Complexity: O(1)
 * 
 * @param tree: Pointer to tree to initialize
 */
void avl_init(AVLTree* tree) {
    if (tree == NULL) {
        return;
    }
    tree->root = NULL;
    tree->size = 0;
}

/**
 * Helper function: Checks for time conflict in subtree
 * Recursively checks all appointments with same doctor_id
 * Time Complexity: O(n) worst case, but typically O(log n) due to tree structure
 * 
 * @param node: Root of subtree to check
 * @param doctor_id: Doctor ID to check conflicts for
 * @param start_time: Proposed start time
 * @param end_time: Proposed end time
 * @return: 1 if conflict exists, 0 otherwise
 */
static int check_conflict_in_subtree(AVLNode* node, int doctor_id, 
                                     int start_time, int end_time) {
    if (node == NULL) {
        return 0; // No conflict in empty subtree
    }

    // Check current node for conflict (same doctor and overlapping times)
    if (node->appointment != NULL && 
        node->appointment->doctor_id == doctor_id) {
        // Check if time intervals overlap
        // Two intervals [a1, b1] and [a2, b2] overlap if: a1 < b2 && a2 < b1
        if (start_time < node->appointment->end_time && 
            node->appointment->start_time < end_time) {
            return 1; // Conflict found
        }
    }

    // Recursively check left and right subtrees
    // We need to check both because conflict detection is by doctor_id,
    // not by start_time (which is the tree key)
    if (check_conflict_in_subtree(node->left, doctor_id, start_time, end_time)) {
        return 1;
    }
    if (check_conflict_in_subtree(node->right, doctor_id, start_time, end_time)) {
        return 1;
    }

    return 0; // No conflict found
}

/**
 * Helper function: Recursive insert into AVL tree
 * Time Complexity: O(log n) for insertion + O(n) for conflict check in worst case
 * 
 * @param node: Root of subtree
 * @param appointment: Appointment to insert
 * @param doctor_id: Doctor ID for conflict checking
 * @param inserted: Pointer to flag indicating if insertion succeeded
 * @return: Pointer to root of subtree after insertion
 */
static AVLNode* avl_insert_recursive(AVLNode* node, Appointment* appointment, 
                                     int doctor_id, int* inserted) {
    // Base case: create new node
    if (node == NULL) {
        AVLNode* new_node = (AVLNode*)malloc(sizeof(AVLNode));
        if (new_node == NULL) {
            *inserted = 0;
            return NULL; // Memory allocation failed
        }

        new_node->appointment = appointment;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 0; // Leaf node has height 0

        *inserted = 1;
        return new_node;
    }

    // Check for conflict before inserting
    // This ensures we don't insert conflicting appointments
    if (check_conflict_in_subtree(node, doctor_id, 
                                  appointment->start_time, 
                                  appointment->end_time)) {
        *inserted = 0; // Conflict detected
        return node; // Return unchanged tree
    }

    // Insert based on start_time (tree key)
    if (appointment->start_time < node->appointment->start_time) {
        node->left = avl_insert_recursive(node->left, appointment, doctor_id, inserted);
    } else if (appointment->start_time > node->appointment->start_time) {
        node->right = avl_insert_recursive(node->right, appointment, doctor_id, inserted);
    } else {
        // Duplicate start_time (shouldn't happen, but handle it)
        *inserted = 0;
        return node;
    }

    // Update height of current node
    avl_update_height(node);

    // Get balance factor
    int balance = avl_get_balance(node);

    // Left Left Case (left-heavy, left child is also left-heavy)
    if (balance > 1 && appointment->start_time < node->left->appointment->start_time) {
        return avl_rotate_right(node);
    }

    // Right Right Case (right-heavy, right child is also right-heavy)
    if (balance < -1 && appointment->start_time > node->right->appointment->start_time) {
        return avl_rotate_left(node);
    }

    // Left Right Case (left-heavy, but left child is right-heavy)
    if (balance > 1 && appointment->start_time > node->left->appointment->start_time) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }

    // Right Left Case (right-heavy, but right child is left-heavy)
    if (balance < -1 && appointment->start_time < node->right->appointment->start_time) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    // Tree is balanced, return unchanged node
    return node;
}

/**
 * Inserts an appointment into AVL tree
 * Automatically balances tree and checks for time conflicts
 * Time Complexity: O(log n) for balanced tree, O(n) worst case for conflict check
 * 
 * @param tree: Pointer to AVL tree
 * @param appointment: Pointer to appointment to insert
 * @param doctor_id: Doctor ID for conflict checking
 * @return: 1 on success, 0 if conflict detected or insertion failed
 */
int avl_insert(AVLTree* tree, Appointment* appointment, int doctor_id) {
    if (tree == NULL || appointment == NULL) {
        return 0;
    }

    int inserted = 0;
    tree->root = avl_insert_recursive(tree->root, appointment, doctor_id, &inserted);

    if (inserted) {
        tree->size++;
        return 1;
    }

    return 0; // Conflict detected or insertion failed
}

/**
 * Helper function: Finds node with minimum start_time in subtree
 * Time Complexity: O(log n)
 * 
 * @param node: Root of subtree
 * @return: Pointer to node with minimum start_time
 */
static AVLNode* find_min_node(AVLNode* node) {
    if (node == NULL || node->left == NULL) {
        return node;
    }
    return find_min_node(node->left);
}

/**
 * Helper function: Delete node by start_time (tree key)
 * Used when deleting inorder successor during 2-child deletion
 * Time Complexity: O(log n)
 * 
 * @param node: Root of subtree
 * @param start_time: Start time to delete
 * @return: Pointer to root of subtree after deletion
 */
static AVLNode* avl_delete_by_time(AVLNode* node, int start_time) {
    if (node == NULL) {
        return NULL;
    }

    if (start_time < node->appointment->start_time) {
        node->left = avl_delete_by_time(node->left, start_time);
    } else if (start_time > node->appointment->start_time) {
        node->right = avl_delete_by_time(node->right, start_time);
    } else {
        // Found node to delete
        // Node with 0 or 1 child
        if (node->left == NULL) {
            AVLNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            AVLNode* temp = node->left;
            free(node);
            return temp;
        }

        // Node with 2 children: get inorder successor
        AVLNode* temp = find_min_node(node->right);
        node->appointment = temp->appointment;
        node->right = avl_delete_by_time(node->right, temp->appointment->start_time);
    }

    // Update height
    avl_update_height(node);

    // Balance the tree
    int balance = avl_get_balance(node);

    // Left Left Case
    if (balance > 1 && avl_get_balance(node->left) >= 0) {
        return avl_rotate_right(node);
    }
    // Left Right Case
    if (balance > 1 && avl_get_balance(node->left) < 0) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }
    // Right Right Case
    if (balance < -1 && avl_get_balance(node->right) <= 0) {
        return avl_rotate_left(node);
    }
    // Right Left Case
    if (balance < -1 && avl_get_balance(node->right) > 0) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    return node;
}

/**
 * Helper function: Recursive delete from AVL tree
 * Note: Tree is keyed by start_time, but we search by appointment_id
 * This requires full tree traversal - O(n) worst case
 * Time Complexity: O(n) worst case (must check all nodes)
 * 
 * @param node: Root of subtree
 * @param appointment_id: ID of appointment to delete
 * @param deleted_appointment: Pointer to store deleted appointment
 * @return: Pointer to root of subtree after deletion
 */
static AVLNode* avl_delete_recursive(AVLNode* node, int appointment_id, 
                                     Appointment** deleted_appointment) {
    if (node == NULL) {
        *deleted_appointment = NULL;
        return NULL; // Appointment not found
    }

    // Search for node to delete by appointment_id
    // Since tree is keyed by start_time, we must check both subtrees
    if (node->appointment->appointment_id == appointment_id) {
        // Found node to delete
        *deleted_appointment = node->appointment;

        // Node with 0 or 1 child
        if (node->left == NULL) {
            AVLNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            AVLNode* temp = node->left;
            free(node);
            return temp;
        }

        // Node with 2 children: get inorder successor (smallest in right subtree)
        AVLNode* temp = find_min_node(node->right);

        // Copy successor's appointment to this node
        node->appointment = temp->appointment;

        // Delete the inorder successor (by start_time since tree is keyed by start_time)
        node->right = avl_delete_by_time(node->right, temp->appointment->start_time);
    } else {
        // Continue searching in both subtrees (tree is keyed by start_time, not appointment_id)
        Appointment* left_result = NULL;
        Appointment* right_result = NULL;
        
        // Try left subtree
        node->left = avl_delete_recursive(node->left, appointment_id, &left_result);
        if (left_result != NULL) {
            *deleted_appointment = left_result;
        } else {
            // Try right subtree
            node->right = avl_delete_recursive(node->right, appointment_id, &right_result);
            if (right_result != NULL) {
                *deleted_appointment = right_result;
            }
        }
        
        // If not found in either subtree, return unchanged
        if (left_result == NULL && right_result == NULL) {
            return node;
        }
    }

    // Update height
    avl_update_height(node);

    // Get balance factor
    int balance = avl_get_balance(node);

    // Left Left Case
    if (balance > 1 && avl_get_balance(node->left) >= 0) {
        return avl_rotate_right(node);
    }

    // Left Right Case
    if (balance > 1 && avl_get_balance(node->left) < 0) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }

    // Right Right Case
    if (balance < -1 && avl_get_balance(node->right) <= 0) {
        return avl_rotate_left(node);
    }

    // Right Left Case
    if (balance < -1 && avl_get_balance(node->right) > 0) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    return node; // Tree is balanced
}

/**
 * Deletes an appointment from AVL tree by appointment_id
 * Automatically balances tree after deletion
 * Time Complexity: O(log n)
 * 
 * @param tree: Pointer to AVL tree
 * @param appointment_id: ID of appointment to delete
 * @return: Pointer to deleted appointment, NULL if not found
 */
Appointment* avl_delete(AVLTree* tree, int appointment_id) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    Appointment* deleted_appointment = NULL;
    tree->root = avl_delete_recursive(tree->root, appointment_id, &deleted_appointment);

    if (deleted_appointment != NULL) {
        tree->size--;
    }

    return deleted_appointment;
}

/**
 * Helper function: Recursive search by appointment_id
 * Time Complexity: O(log n) average, O(n) worst case if tree is unbalanced
 * 
 * @param node: Root of subtree
 * @param appointment_id: ID to search for
 * @return: Pointer to appointment if found, NULL otherwise
 */
static Appointment* avl_search_by_id_recursive(AVLNode* node, int appointment_id) {
    if (node == NULL) {
        return NULL;
    }

    if (appointment_id < node->appointment->appointment_id) {
        return avl_search_by_id_recursive(node->left, appointment_id);
    } else if (appointment_id > node->appointment->appointment_id) {
        return avl_search_by_id_recursive(node->right, appointment_id);
    } else {
        return node->appointment; // Found
    }
}

/**
 * Searches for an appointment by appointment_id
 * Time Complexity: O(log n) due to AVL balancing
 * 
 * @param tree: Pointer to AVL tree
 * @param appointment_id: ID to search for
 * @return: Pointer to appointment if found, NULL otherwise
 */
Appointment* avl_search_by_id(AVLTree* tree, int appointment_id) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    return avl_search_by_id_recursive(tree->root, appointment_id);
}

/**
 * Helper function: Recursive range search
 * Collects appointments in time range for specific doctor
 * Time Complexity: O(n) worst case (visits all nodes), but typically O(log n + k) 
 *                  where k is number of results
 * 
 * @param node: Root of subtree
 * @param doctor_id: Doctor ID to search for
 * @param start_time: Start of time range
 * @param end_time: End of time range
 * @param results: Array to store results
 * @param max_results: Maximum number of results
 * @param count: Pointer to current count of results
 */
static void avl_search_range_recursive(AVLNode* node, int doctor_id,
                                      int start_time, int end_time,
                                      Appointment** results, int max_results,
                                      int* count) {
    if (node == NULL || *count >= max_results) {
        return;
    }

    // In-order traversal with pruning
    // Check left subtree if it might contain valid appointments
    if (node->appointment->start_time >= start_time) {
        avl_search_range_recursive(node->left, doctor_id, start_time, end_time,
                                   results, max_results, count);
    }

    // Check current node
    if (*count < max_results &&
        node->appointment->doctor_id == doctor_id &&
        node->appointment->start_time >= start_time &&
        node->appointment->start_time <= end_time) {
        results[*count] = node->appointment;
        (*count)++;
    }

    // Check right subtree if it might contain valid appointments
    if (node->appointment->start_time <= end_time) {
        avl_search_range_recursive(node->right, doctor_id, start_time, end_time,
                                   results, max_results, count);
    }
}

/**
 * Searches for appointments in a given time range
 * Time Complexity: O(log n + k) where k is number of results
 * 
 * @param tree: Pointer to AVL tree
 * @param doctor_id: Doctor ID to search for
 * @param start_time: Start of time range
 * @param end_time: End of time range
 * @param results: Array to store found appointments (must be pre-allocated)
 * @param max_results: Maximum number of results
 * @return: Number of appointments found
 */
int avl_search_range(AVLTree* tree, int doctor_id, int start_time, 
                     int end_time, Appointment** results, int max_results) {
    if (tree == NULL || tree->root == NULL || results == NULL || max_results <= 0) {
        return 0;
    }

    int count = 0;
    avl_search_range_recursive(tree->root, doctor_id, start_time, end_time,
                              results, max_results, &count);
    return count;
}

/**
 * Checks if an appointment time conflicts with existing appointments
 * for the same doctor
 * Time Complexity: O(n) worst case (checks all nodes), but typically O(log n)
 * 
 * @param tree: Pointer to AVL tree
 * @param doctor_id: Doctor ID to check
 * @param start_time: Proposed start time
 * @param end_time: Proposed end time
 * @return: 1 if conflict exists, 0 otherwise
 */
int avl_has_conflict(AVLTree* tree, int doctor_id, int start_time, int end_time) {
    if (tree == NULL || tree->root == NULL) {
        return 0; // No conflict in empty tree
    }

    return check_conflict_in_subtree(tree->root, doctor_id, start_time, end_time);
}

/**
 * Returns the number of appointments in tree
 * Time Complexity: O(1)
 * 
 * @param tree: Pointer to AVL tree
 * @return: Size of tree
 */
int avl_size(const AVLTree* tree) {
    if (tree == NULL) {
        return 0;
    }
    return tree->size;
}

/**
 * Checks if tree is empty
 * Time Complexity: O(1)
 * 
 * @param tree: Pointer to AVL tree
 * @return: 1 if empty, 0 otherwise
 */
int avl_is_empty(const AVLTree* tree) {
    if (tree == NULL) {
        return 1;
    }
    return (tree->root == NULL) ? 1 : 0;
}

/**
 * Helper function: Recursive destroy
 * Frees all nodes in subtree
 * Time Complexity: O(n)
 * 
 * @param node: Root of subtree to destroy
 */
static void avl_destroy_recursive(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    // Post-order traversal: destroy children first, then node
    avl_destroy_recursive(node->left);
    avl_destroy_recursive(node->right);

    // Note: We don't free appointments here - they are managed elsewhere
    free(node);
}

/**
 * Frees all memory allocated for the AVL tree
 * Time Complexity: O(n) where n is number of nodes
 * 
 * @param tree: Pointer to tree to destroy
 */
void avl_destroy(AVLTree* tree) {
    if (tree == NULL) {
        return;
    }

    avl_destroy_recursive(tree->root);
    tree->root = NULL;
    tree->size = 0;
}


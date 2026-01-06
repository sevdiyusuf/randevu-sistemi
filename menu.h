#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "avl.h"
#include "heap.h"
#include "queue.h"
#include "stack.h"

/**
 * Menu System
 * Provides CLI-based interface for hospital appointment management
 */

/**
 * Displays the main menu options
 */
void display_menu(void);

/**
 * Processes user menu selection
 * @param choice: User's menu choice
 * @param avl_tree: Pointer to AVL tree for appointments
 * @param heap: Pointer to min-heap for earliest appointments
 * @param queue: Pointer to queue for waiting list
 * @param stack: Pointer to stack for undo operations
 * @param patients: Array of patient pointers
 * @param patient_count: Number of patients
 * @param doctors: Array of doctor pointers
 * @param doctor_count: Number of doctors
 * @param appointments: Array of appointment pointers
 * @param appointment_count: Number of appointments
 * @return: 1 to continue, 0 to exit
 */
int process_menu_choice(int choice, AVLTree* avl_tree, MinHeap* heap, 
                        Queue* queue, Stack* stack,
                        Patient** patients, int* patient_count,
                        Doctor** doctors, int* doctor_count,
                        Appointment** appointments, int* appointment_count);

/**
 * Menu Option Handlers
 */

/**
 * Handles adding a new patient
 * @param patients: Array of patient pointers
 * @param patient_count: Pointer to current patient count
 */
void menu_add_patient(Patient** patients, int* patient_count);

/**
 * Handles adding a new doctor
 * @param doctors: Array of doctor pointers
 * @param doctor_count: Pointer to current doctor count
 */
void menu_add_doctor(Doctor** doctors, int* doctor_count);

/**
 * Handles creating a new appointment
 * Checks for conflicts and manages waiting list
 * @param avl_tree: Pointer to AVL tree
 * @param heap: Pointer to min-heap
 * @param queue: Pointer to waiting queue
 * @param appointments: Array of appointment pointers
 * @param appointment_count: Pointer to current appointment count
 * @param patients: Array of patient pointers
 * @param patient_count: Number of patients
 * @param doctors: Array of doctor pointers
 * @param doctor_count: Number of doctors
 */
void menu_create_appointment(AVLTree* avl_tree, MinHeap* heap, Queue* queue,
                            Appointment** appointments, int* appointment_count,
                            Patient** patients, int patient_count,
                            Doctor** doctors, int doctor_count);

/**
 * Handles cancelling an appointment
 * Pushes cancelled appointment to undo stack
 * @param avl_tree: Pointer to AVL tree
 * @param heap: Pointer to min-heap
 * @param stack: Pointer to undo stack
 * @param appointments: Array of appointment pointers
 * @param appointment_count: Pointer to current appointment count
 */
void menu_cancel_appointment(AVLTree* avl_tree, MinHeap* heap, Stack* stack,
                            Appointment** appointments, int* appointment_count);

/**
 * Handles undoing the last cancellation
 * Restores appointment from undo stack
 * @param avl_tree: Pointer to AVL tree
 * @param heap: Pointer to min-heap
 * @param stack: Pointer to undo stack
 * @param appointments: Array of appointment pointers
 * @param appointment_count: Pointer to current appointment count
 */
void menu_undo_cancel(AVLTree* avl_tree, MinHeap* heap, Stack* stack,
                     Appointment** appointments, int* appointment_count);

/**
 * Displays waiting queue (appointments waiting for available slots)
 * @param queue: Pointer to waiting queue
 */
void menu_show_waiting_queue(Queue* queue);

/**
 * Displays the earliest appointment using min-heap
 * @param heap: Pointer to min-heap
 */
void menu_show_earliest_appointment(MinHeap* heap);

/**
 * Lists all appointments sorted by start time
 * Uses MergeSort for sorting
 * @param appointments: Array of appointment pointers
 * @param appointment_count: Number of appointments
 * @param patients: Array of patient pointers
 * @param patient_count: Number of patients
 * @param doctors: Array of doctor pointers
 * @param doctor_count: Number of doctors
 */
void menu_list_appointments(Appointment** appointments, int appointment_count,
                           Patient** patients, int patient_count,
                           Doctor** doctors, int doctor_count);

/**
 * Utility Functions
 */

/**
 * Gets integer input from user with validation
 * @param prompt: Prompt message to display
 * @param min: Minimum allowed value
 * @param max: Maximum allowed value
 * @return: Valid integer input
 */
int get_int_input(const char* prompt, int min, int max);

/**
 * Gets string input from user
 * @param prompt: Prompt message to display
 * @param buffer: Buffer to store input
 * @param buffer_size: Size of buffer
 */
void get_string_input(const char* prompt, char* buffer, int buffer_size);

/**
 * Clears input buffer
 */
void clear_input_buffer(void);

#endif // MENU_H


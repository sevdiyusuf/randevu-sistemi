#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "avl.h"
#include "heap.h"
#include "queue.h"
#include "stack.h"
#include "file_io.h"

/**
 * GUI Module Header
 * 
 * GTK3-based graphical user interface for Hospital Appointment Management System
 * This module provides a GUI layer on top of existing core logic.
 * 
 * Architecture:
 *   GUI (gui.c) -> Core Logic (appointment.c, avl.c, etc.) -> CSV Files
 */

/**
 * Application data structure
 * Holds references to all core data structures for GUI access
 */
typedef struct {
    Patient** patients;
    int* patient_count;
    Doctor** doctors;
    int* doctor_count;
    Appointment** appointments;
    int* appointment_count;
    AVLTree* avl_tree;
    MinHeap* heap;
    Queue* queue;
    Stack* stack;
} AppData;

/**
 * Starts the GTK GUI application
 * 
 * @param argc: Command line argument count
 * @param argv: Command line arguments array
 * @param app_data: Pointer to application data structure
 */
void start_gui(int argc, char *argv[], AppData* app_data);

#endif // GUI_H


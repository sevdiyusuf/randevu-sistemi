#ifndef DOCTOR_H
#define DOCTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Doctor Data Structure
 * Represents a doctor in the hospital system
 */
typedef struct {
    int id;              // Unique doctor identifier
    char name[100];      // Doctor's full name
    char department[50]; // Department name (e.g., "Cardiology", "Pediatrics")
} Doctor;

/**
 * Doctor Management Functions
 */

/**
 * Creates a new doctor with given parameters
 * @param id: Unique doctor ID
 * @param name: Doctor's name
 * @param department: Department name
 * @return: Pointer to newly created Doctor, NULL on failure
 */
Doctor* create_doctor(int id, const char* name, const char* department);

/**
 * Frees memory allocated for a doctor
 * @param doctor: Pointer to doctor to free
 */
void free_doctor(Doctor* doctor);

/**
 * Displays doctor information
 * @param doctor: Pointer to doctor to display
 */
void display_doctor(const Doctor* doctor);

/**
 * Validates doctor data
 * @param doctor: Pointer to doctor to validate
 * @return: 1 if valid, 0 otherwise
 */
int validate_doctor(const Doctor* doctor);

#endif // DOCTOR_H


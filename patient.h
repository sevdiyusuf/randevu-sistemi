#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Patient Data Structure
 * Represents a patient in the hospital system
 */
typedef struct {
    int id;              // Unique patient identifier
    char name[100];      // Patient's full name
    int age;             // Patient's age
} Patient;

/**
 * Patient Management Functions
 */

/**
 * Creates a new patient with given parameters
 * @param id: Unique patient ID
 * @param name: Patient's name
 * @param age: Patient's age
 * @return: Pointer to newly created Patient, NULL on failure
 */
Patient* create_patient(int id, const char* name, int age);

/**
 * Frees memory allocated for a patient
 * @param patient: Pointer to patient to free
 */
void free_patient(Patient* patient);

/**
 * Displays patient information
 * @param patient: Pointer to patient to display
 */
void display_patient(const Patient* patient);

/**
 * Validates patient data
 * @param patient: Pointer to patient to validate
 * @return: 1 if valid, 0 otherwise
 */
int validate_patient(const Patient* patient);

#endif // PATIENT_H


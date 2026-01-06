#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "doctor.h"

/**
 * Appointment Data Structure
 * Represents an appointment between a patient and doctor
 * Time is stored as minutes from midnight (0-1439)
 */
typedef struct {
    int appointment_id;  // Unique appointment identifier
    int patient_id;      // Reference to patient ID
    int doctor_id;       // Reference to doctor ID
    int start_time;      // Start time in minutes from midnight (0-1439)
    int end_time;        // End time in minutes from midnight (0-1439)
} Appointment;

/**
 * Appointment Management Functions
 */

/**
 * Creates a new appointment with given parameters
 * @param appointment_id: Unique appointment ID
 * @param patient_id: Patient ID
 * @param doctor_id: Doctor ID
 * @param start_time: Start time in minutes from midnight
 * @param end_time: End time in minutes from midnight
 * @return: Pointer to newly created Appointment, NULL on failure
 */
Appointment* create_appointment(int appointment_id, int patient_id, 
                                 int doctor_id, int start_time, int end_time);

/**
 * Frees memory allocated for an appointment
 * @param appointment: Pointer to appointment to free
 */
void free_appointment(Appointment* appointment);

/**
 * Displays appointment information
 * @param appointment: Pointer to appointment to display
 */
void display_appointment(const Appointment* appointment);

/**
 * Validates appointment data and time range
 * @param appointment: Pointer to appointment to validate
 * @return: 1 if valid, 0 otherwise
 */
int validate_appointment(const Appointment* appointment);

/**
 * Checks if two appointments have overlapping time intervals
 * Used for conflict detection
 * @param a1: First appointment
 * @param a2: Second appointment
 * @return: 1 if overlapping, 0 otherwise
 */
int appointments_overlap(const Appointment* a1, const Appointment* a2);

/**
 * Converts minutes from midnight to HH:MM format string
 * @param minutes: Time in minutes from midnight
 * @param buffer: Buffer to store formatted string (must be at least 6 chars)
 */
void minutes_to_time_string(int minutes, char* buffer);

#endif // APPOINTMENT_H


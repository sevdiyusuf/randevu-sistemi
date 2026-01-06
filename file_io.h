#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"

/**
 * File I/O Operations
 * Handles CSV file persistence for patients, doctors, and appointments
 */

/**
 * Maximum number of records that can be loaded from CSV
 */
#define MAX_RECORDS 1000

/**
 * Patient File Operations
 */

/**
 * Loads patients from CSV file
 * @param filename: Path to patients CSV file
 * @param patients: Array to store loaded patients (must be pre-allocated)
 * @param max_count: Maximum number of patients to load
 * @return: Number of patients successfully loaded
 */
int load_patients_from_csv(const char* filename, Patient** patients, int max_count);

/**
 * Saves patients to CSV file
 * @param filename: Path to patients CSV file
 * @param patients: Array of patient pointers
 * @param count: Number of patients to save
 * @return: Number of patients successfully saved
 */
int save_patients_to_csv(const char* filename, Patient** patients, int count);

/**
 * Doctor File Operations
 */

/**
 * Loads doctors from CSV file
 * @param filename: Path to doctors CSV file
 * @param doctors: Array to store loaded doctors (must be pre-allocated)
 * @param max_count: Maximum number of doctors to load
 * @return: Number of doctors successfully loaded
 */
int load_doctors_from_csv(const char* filename, Doctor** doctors, int max_count);

/**
 * Saves doctors to CSV file
 * @param filename: Path to doctors CSV file
 * @param doctors: Array of doctor pointers
 * @param count: Number of doctors to save
 * @return: Number of doctors successfully saved
 */
int save_doctors_to_csv(const char* filename, Doctor** doctors, int count);

/**
 * Appointment File Operations
 */

/**
 * Loads appointments from CSV file
 * @param filename: Path to appointments CSV file
 * @param appointments: Array to store loaded appointments (must be pre-allocated)
 * @param max_count: Maximum number of appointments to load
 * @return: Number of appointments successfully loaded
 */
int load_appointments_from_csv(const char* filename, Appointment** appointments, int max_count);

/**
 * Saves appointments to CSV file
 * @param filename: Path to appointments CSV file
 * @param appointments: Array of appointment pointers
 * @param count: Number of appointments to save
 * @return: Number of appointments successfully saved
 */
int save_appointments_to_csv(const char* filename, Appointment** appointments, int count);

/**
 * Utility Functions
 */

/**
 * Parses a time string (HH:MM) to minutes from midnight
 * @param time_str: Time string in HH:MM format
 * @return: Minutes from midnight, -1 on error
 */
int parse_time_string(const char* time_str);

/**
 * Creates a time string (HH:MM) from minutes from midnight
 * @param minutes: Minutes from midnight
 * @param buffer: Buffer to store formatted string (must be at least 6 chars)
 */
void format_time_string(int minutes, char* buffer);

/**
 * Automatic ID Generation Functions
 * These functions manage sequential ID generation for patients, doctors, and appointments
 */

/**
 * Gets the next available patient ID and increments the counter
 * @return: Next patient ID (starts from 1)
 */
int get_next_patient_id(void);

/**
 * Gets the next available doctor ID and increments the counter
 * @return: Next doctor ID (starts from 1)
 */
int get_next_doctor_id(void);

/**
 * Gets the next available appointment ID and increments the counter
 * @return: Next appointment ID (starts from 1)
 */
int get_next_appointment_id(void);

/**
 * Updates patient ID counter based on existing patients
 * Should be called after loading patients from CSV
 * @param patients: Array of patient pointers
 * @param count: Number of patients
 */
void update_patient_id_counter(Patient** patients, int count);

/**
 * Updates doctor ID counter based on existing doctors
 * Should be called after loading doctors from CSV
 * @param doctors: Array of doctor pointers
 * @param count: Number of doctors
 */
void update_doctor_id_counter(Doctor** doctors, int count);

/**
 * Updates appointment ID counter based on existing appointments
 * Should be called after loading appointments from CSV
 * @param appointments: Array of appointment pointers
 * @param count: Number of appointments
 */
void update_appointment_id_counter(Appointment** appointments, int count);

#endif // FILE_IO_H


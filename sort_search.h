#ifndef SORT_SEARCH_H
#define SORT_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include "appointment.h"
#include "patient.h"
#include "doctor.h"

/**
 * Sorting and Searching Algorithms
 * Implements MergeSort, HeapSort, and Binary Search
 */

/**
 * Sorting Algorithms
 */

/**
 * MergeSort for appointments (sorted by start_time)
 * Time Complexity: O(n log n) - stable, divide and conquer
 * Space Complexity: O(n) - requires temporary array
 * @param appointments: Array of appointment pointers
 * @param left: Left index of subarray
 * @param right: Right index of subarray
 */
void merge_sort_appointments(Appointment** appointments, int left, int right);

/**
 * HeapSort for appointments (sorted by start_time)
 * Time Complexity: O(n log n) - in-place sorting
 * Space Complexity: O(1) - no extra space needed
 * @param appointments: Array of appointment pointers
 * @param n: Number of appointments
 */
void heap_sort_appointments(Appointment** appointments, int n);

/**
 * MergeSort for patients (sorted by id)
 * Time Complexity: O(n log n)
 * @param patients: Array of patient pointers
 * @param left: Left index of subarray
 * @param right: Right index of subarray
 */
void merge_sort_patients(Patient** patients, int left, int right);

/**
 * MergeSort for doctors (sorted by id)
 * Time Complexity: O(n log n)
 * @param doctors: Array of doctor pointers
 * @param left: Left index of subarray
 * @param right: Right index of subarray
 */
void merge_sort_doctors(Doctor** doctors, int left, int right);

/**
 * Searching Algorithms
 */

/**
 * Binary Search for appointments by appointment_id
 * Requires array to be sorted by appointment_id first
 * Time Complexity: O(log n)
 * @param appointments: Sorted array of appointment pointers
 * @param n: Number of appointments
 * @param appointment_id: ID to search for
 * @return: Index of found appointment, -1 if not found
 */
int binary_search_appointment_by_id(Appointment** appointments, int n, int appointment_id);

/**
 * Binary Search for patients by patient_id
 * Requires array to be sorted by patient_id first
 * Time Complexity: O(log n)
 * @param patients: Sorted array of patient pointers
 * @param n: Number of patients
 * @param patient_id: ID to search for
 * @return: Index of found patient, -1 if not found
 */
int binary_search_patient_by_id(Patient** patients, int n, int patient_id);

/**
 * Binary Search for doctors by doctor_id
 * Requires array to be sorted by doctor_id first
 * Time Complexity: O(log n)
 * @param doctors: Sorted array of doctor pointers
 * @param n: Number of doctors
 * @param doctor_id: ID to search for
 * @return: Index of found doctor, -1 if not found
 */
int binary_search_doctor_by_id(Doctor** doctors, int n, int doctor_id);

/**
 * Helper function: Merges two sorted subarrays (for MergeSort)
 * Time Complexity: O(n)
 */
void merge_appointments(Appointment** appointments, int left, int mid, int right);

/**
 * Helper function: Merges two sorted patient subarrays
 * Time Complexity: O(n)
 */
void merge_patients(Patient** patients, int left, int mid, int right);

/**
 * Helper function: Merges two sorted doctor subarrays
 * Time Complexity: O(n)
 */
void merge_doctors(Doctor** doctors, int left, int mid, int right);

#endif // SORT_SEARCH_H


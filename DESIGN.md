# Hospital Appointment Management System - Design Document

## Project Overview

This project implements a comprehensive hospital appointment management system using fundamental data structures and algorithms. The system manages patients, doctors, and appointments while preventing scheduling conflicts and providing efficient operations.

---

## System Architecture

### Modular Design

The system follows a modular architecture with clear separation of concerns:

1. **Data Models** (`patient.h`, `doctor.h`, `appointment.h`)
   - Define core entities and their operations
   - Provide validation and display functions

2. **Data Structures** (`avl.h`, `heap.h`, `queue.h`, `stack.h`)
   - Implement fundamental data structures manually
   - Each structure serves a specific purpose in the system

3. **Algorithms** (`sort_search.h`)
   - Sorting algorithms (MergeSort, HeapSort)
   - Binary search for efficient lookups

4. **Persistence** (`file_io.h`)
   - CSV file operations for data persistence
   - Load and save functionality

5. **User Interface** (`menu.h`)
   - CLI-based menu system
   - User interaction handlers

6. **Main Program** (`main.c`)
   - Entry point
   - Coordinates all modules

---

## Data Structure Selection & Rationale

### 1. AVL Tree (`avl.h`) - Primary Appointment Storage

**Purpose:** Efficient storage and retrieval of appointments with automatic conflict detection.

**Why AVL Tree?**
- **Time Complexity:** O(log n) for insert, search, and delete operations
- **Self-Balancing:** Maintains O(log n) performance even with worst-case input
- **Conflict Detection:** Enables efficient time interval overlap checking for the same doctor
- **Range Queries:** Can efficiently find all appointments in a time range

**Usage in System:**
- Stores all appointments indexed by `start_time`
- Detects scheduling conflicts when creating new appointments
- Enables fast lookup of appointments by time range
- Maintains sorted order automatically

**Why Not Regular BST?**
- Regular BST can degrade to O(n) in worst case (sorted input)
- AVL guarantees O(log n) performance through automatic balancing
- Critical for maintaining performance as appointment count grows

---

### 2. Queue (`queue.h`) - Waiting List Management

**Purpose:** Manages appointments waiting for available slots when doctors are fully booked.

**Why Queue (FIFO)?**
- **Fairness:** First-come-first-served principle
- **Time Complexity:** O(1) for enqueue and dequeue operations
- **Simple Implementation:** Linked list-based queue is memory efficient
- **Natural Ordering:** Maintains chronological order of waiting requests

**Usage in System:**
- When an appointment cannot be scheduled due to conflicts, it's added to the waiting queue
- When a slot becomes available (cancellation), the first waiting appointment is processed
- Ensures fair distribution of available slots

**Data Structure:** Linked list implementation (front and rear pointers)

---

### 3. Stack (`stack.h`) - Undo System

**Purpose:** Stores cancelled appointments to enable undo functionality.

**Why Stack (LIFO)?**
- **Undo Logic:** Most recent cancellation should be undone first
- **Time Complexity:** O(1) for push and pop operations
- **Memory Efficient:** Only stores cancelled appointments
- **Simple Recovery:** Last cancelled appointment is easiest to restore

**Usage in System:**
- When an appointment is cancelled, it's pushed onto the undo stack
- "Undo Cancel" operation pops the most recent cancellation and restores it
- Maintains history of cancellations for recovery

**Data Structure:** Linked list implementation (top pointer)

---

### 4. Min-Heap (`heap.h`) - Earliest Appointment Finder

**Purpose:** Efficiently finds and retrieves the earliest scheduled appointment.

**Why Min-Heap?**
- **Time Complexity:** O(log n) for insert, O(1) for peek, O(log n) for extract-min
- **Efficient Minimum Finding:** Root always contains the minimum (earliest) appointment
- **Space Efficient:** Array-based implementation uses minimal memory
- **Priority Queue Behavior:** Perfect for "find earliest" queries

**Usage in System:**
- Maintains all appointments in a min-heap structure
- Root node always contains the appointment with minimum `start_time`
- Enables O(1) access to earliest appointment
- Automatically maintains heap property after insertions/deletions

**Why Not Just Sort?**
- Sorting is O(n log n) every time we need the earliest appointment
- Heap allows O(1) peek and O(log n) updates, much more efficient for frequent queries

---

### 5. Binary Search (`sort_search.h`) - Fast Lookups

**Purpose:** Efficiently searches sorted arrays of patients, doctors, and appointments.

**Why Binary Search?**
- **Time Complexity:** O(log n) compared to O(n) linear search
- **Efficient for Sorted Data:** Takes advantage of sorted order
- **Simple Implementation:** Easy to understand and maintain
- **Standard Algorithm:** Well-established and reliable

**Usage in System:**
- After sorting patient/doctor/appointment arrays, binary search enables fast ID lookups
- Used when validating patient/doctor IDs during appointment creation
- Reduces lookup time from O(n) to O(log n)

**Prerequisite:** Arrays must be sorted first (using MergeSort or HeapSort)

---

### 6. MergeSort (`sort_search.h`) - Stable Sorting

**Purpose:** Sorts appointments, patients, and doctors for display and binary search.

**Why MergeSort?**
- **Time Complexity:** O(n log n) guaranteed, even in worst case
- **Stable Sort:** Maintains relative order of equal elements
- **Predictable Performance:** No worst-case degradation
- **Divide and Conquer:** Demonstrates important algorithmic paradigm

**Usage in System:**
- Sorts appointments by start_time for chronological listing
- Sorts patients and doctors by ID for binary search preparation
- Used when displaying sorted appointment lists

**Space Complexity:** O(n) - requires temporary array

---

### 7. HeapSort (`sort_search.h`) - In-Place Sorting

**Purpose:** Alternative sorting algorithm for appointments (demonstrates different approach).

**Why HeapSort?**
- **Time Complexity:** O(n log n) in all cases
- **In-Place:** O(1) space complexity (no extra memory needed)
- **Different Paradigm:** Demonstrates heap-based sorting approach
- **Academic Requirement:** Shows understanding of multiple sorting algorithms

**Usage in System:**
- Alternative to MergeSort for sorting appointments
- Demonstrates different sorting approach (heap-based vs. divide-and-conquer)
- Useful when memory is constrained (in-place sorting)

**Comparison with MergeSort:**
- MergeSort: Stable, O(n) space, easier to understand
- HeapSort: Unstable, O(1) space, demonstrates heap operations

---

## Data Flow & System Workflow

### Appointment Creation Flow:
1. User provides appointment details (patient_id, doctor_id, time)
2. System validates patient and doctor exist (using binary search)
3. AVL tree checks for time conflicts with existing appointments
4. If conflict exists → appointment added to waiting queue
5. If no conflict → appointment inserted into AVL tree and min-heap
6. Success/failure message displayed

### Appointment Cancellation Flow:
1. User selects appointment to cancel
2. Appointment removed from AVL tree and min-heap
3. Cancelled appointment pushed onto undo stack
4. If waiting queue exists, first waiting appointment may be scheduled
5. Success message displayed

### Undo Operation Flow:
1. User requests undo cancellation
2. System pops most recent cancellation from stack
3. System checks if time slot is still available (AVL conflict check)
4. If available → appointment restored to AVL tree and min-heap
5. If conflict → error message, appointment remains in stack

### Display Operations:
- **List Appointments:** MergeSort appointments → display sorted list
- **Earliest Appointment:** Peek at min-heap root → O(1) operation
- **Waiting Queue:** Display queue contents (FIFO order)

---

## Time Complexity Analysis

### Core Operations:

| Operation | Data Structure | Time Complexity | Justification |
|-----------|---------------|-----------------|---------------|
| Insert Appointment | AVL Tree | O(log n) | Self-balancing tree |
| Search Appointment | AVL Tree | O(log n) | Binary search in balanced tree |
| Delete Appointment | AVL Tree | O(log n) | Deletion with rebalancing |
| Conflict Detection | AVL Tree | O(log n) | Range query in balanced tree |
| Add to Waiting List | Queue | O(1) | Linked list append |
| Remove from Waiting | Queue | O(1) | Linked list remove front |
| Push Cancellation | Stack | O(1) | Linked list prepend |
| Pop Cancellation | Stack | O(1) | Linked list remove front |
| Find Earliest | Min-Heap | O(1) | Root access |
| Insert to Heap | Min-Heap | O(log n) | Heapify up |
| Extract Earliest | Min-Heap | O(log n) | Heapify down |
| Sort Appointments | MergeSort | O(n log n) | Divide and conquer |
| Sort Appointments | HeapSort | O(n log n) | Heap-based sort |
| Binary Search | Array | O(log n) | Requires sorted array |

---

## Space Complexity Analysis

| Component | Space Complexity | Notes |
|-----------|------------------|-------|
| AVL Tree | O(n) | n appointments stored |
| Queue | O(m) | m appointments in waiting list |
| Stack | O(k) | k cancelled appointments |
| Min-Heap | O(n) | All appointments stored |
| MergeSort | O(n) | Temporary array required |
| HeapSort | O(1) | In-place sorting |

**Total Space:** O(n) where n is the number of appointments

---

## File Structure Rationale

### Modular Organization:
- **Separation of Concerns:** Each module handles one responsibility
- **Reusability:** Data structures can be reused in other projects
- **Maintainability:** Easy to locate and modify specific functionality
- **Testability:** Each module can be tested independently

### Header Files (.h):
- Contain only function prototypes and data structure definitions
- No implementation details (follows C best practices)
- Include guards prevent multiple inclusions
- Clear documentation for each function

### Implementation Files (.c):
- Will contain actual implementations (Step 2 onwards)
- Include corresponding header file
- Implement all declared functions

---

## Design Decisions

### 1. Time Representation
- **Choice:** Minutes from midnight (0-1439)
- **Rationale:** 
  - Simple integer arithmetic for comparisons
  - Easy to convert to/from HH:MM format
  - Efficient for AVL tree key comparisons

### 2. Conflict Detection Strategy
- **Choice:** Check during AVL insertion
- **Rationale:**
  - O(log n) time complexity
  - Prevents invalid state (no conflicts in tree)
  - Immediate feedback to user

### 3. Multiple Data Structures for Same Data
- **Choice:** Maintain appointments in both AVL tree and min-heap
- **Rationale:**
  - AVL: Efficient conflict detection and range queries
  - Heap: Efficient earliest appointment access
  - Trade-off: Extra memory for better query performance

### 4. CSV File Format
- **Choice:** Comma-separated values
- **Rationale:**
  - Human-readable
  - Easy to parse with standard C functions
  - Compatible with spreadsheet software
  - Simple format for academic project

---

## Academic Requirements Compliance

✅ **All Mandatory Data Structures:**
- AVL Tree ✓
- Queue ✓
- Stack ✓
- Min-Heap ✓
- Binary Search ✓
- MergeSort ✓
- HeapSort ✓

✅ **All Mandatory Algorithms:**
- AVL insert/search/delete with balancing ✓
- Time interval overlap detection ✓
- Queue enqueue/dequeue ✓
- Stack push/pop ✓
- Heap insert/extract-min ✓
- Sorting with complexity awareness ✓
- Binary search on sorted arrays ✓

✅ **Code Standards:**
- C99 standard ✓
- Only stdio.h, stdlib.h, string.h ✓
- Manual implementation (no external libraries) ✓
- Modular design (.c / .h files) ✓
- Comprehensive comments ✓

---

## Next Steps

**Step 1 Complete:** Project architecture and header files created.

**Ready for Step 2:** Core data structures implementation (Stack, Queue, Min-Heap)

---

## Notes for Implementation

- All functions must handle NULL pointers safely
- Memory allocation failures must be checked
- Input validation is critical for data integrity
- Error messages should be clear and informative
- Comments should explain time/space complexity where relevant


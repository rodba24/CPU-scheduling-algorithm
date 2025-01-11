# CPU Scheduling Algorithms

This project implements a **multi-threaded CPU scheduling simulator** using C and POSIX threads (`pthread`). It simulates three different scheduling algorithms: **FCFS (First-Come, First-Served)**, **SJF (Shortest Job First)**, and **Priority Scheduling**. Each scheduling algorithm runs in its own thread, and the results, including Gantt charts and average times, are displayed.

---

## Features

- **FCFS (First-Come, First-Served):**
  - Processes are executed in the order they arrive.
  - No preemption is used.

- **SJF (Shortest Job First):**
  - Processes with the shortest burst time are executed first.
  - Non-preemptive scheduling is implemented.

- **Priority Scheduling:**
  - Processes are executed based on their priority.
  - Higher priority (greater value) processes are executed first.
  - Handles idle periods and arrival times dynamically.

- **Multi-threaded Execution:**
  - Each algorithm runs in a separate thread, ensuring concurrency.

- **Gantt Chart Visualization:**
  - Visualizes the execution order and time intervals for each process.

- **Performance Metrics:**
  - Calculates and displays:
    - Average Waiting Time (AWT)
    - Average Turnaround Time (TAT)

---

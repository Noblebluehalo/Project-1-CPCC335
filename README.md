# Project 1 – Algorithms Implementation

**Author:** Joshua Zamora ([JoshuaMZamora@csu.fullerton.edu](mailto:JoshuaMZamora@csu.fullerton.edu))  
**Course:** CPSC 335 – Algorithm Engineering (Fall 2025)

---

## Overview

This project implements two classic algorithmic problems in **C++**:

### 1. Alternating Disk Problem
Sort a row of Light (L) and Dark (D) disks so that all **Darks** are on the left and all **Lights** are on the right using only adjacent swaps.  
*Implemented in [`algorithm1.cpp`](algorithm1.cpp).*

### 2. Group Meeting Scheduler
Given the daily availability windows and busy schedules of multiple people, find all possible **meeting slots** of at least a specified duration.  
*Implemented in [`algorithm2.cpp`](algorithm2.cpp).*

---

**File Structure**

- Algorithm 1 (Alternating Disk Problem)
- Algorithm 2 (Group Meeting Scheduler)
- README.md

---

## Algorithm 1 – Alternating Disk Problem

**Goal**  
Rearrange a user-provided sequence of Light (L) and Dark (D) disks so that all **D** are on the left and all **L** are on the right.

**How It Works**
- Accepts any starting sequence of disks entered by the user.
- Repeatedly scans the row **left-to-right** and **right-to-left**, swapping any `L D` pairs.
- Each swap removes an inversion until the sequence becomes fully sorted (`D … D L … L`).

**Run Example**
```bash
g++ -std=c++17 -O2 -Wall -Wextra -o algorithm1 algorithm1.cpp
./algorithm1

Enter total number of disks: 6
Enter the sequence of disks (L for Light, D for Dark) separated by spaces:
L D L D D L

Final arrangement: D D D L L L 
Total swaps performed: 6
Directional passes (left-to-right or right-to-left): 6
```
## Algorithm 2 – Group Meeting Scheduler

**Goal**  
Find time slots when **everyone is free**, given each person’s **busy schedule** and **daily active availability**.

**How It Works**
1. Convert all time strings (`HH: MM`) into **minutes since midnight**.
2. Treat time **outside each person’s active window** as busy.
3. Merge all busy intervals across everyone.
4. Invert the merged busy timeline to get global **free slots**.
5. Clip free slots to the **shared active window** and filter by the **minimum required meeting duration**.

**Run Example**
```bash
g++ -std=c++17 -O2 -Wall -Wextra -o algorithm2 algorithm2.cpp
./algorithm2

Enter number of people: 2
Person 1 - number of busy intervals: 3
7:00 8:30
12:00 13:00
16:00 18:00
Enter daily active window 'HH: MM HH: MM' (earliest, latest):09:00 19:00
Person 2 - number of busy intervals: 4
9:00 10:30
12:20 14:00
14:30 15:00
16:00 17:00
Enter daily active window 'HH: MM HH: MM' (earliest, latest): 09:00 18:30
Enter required meeting duration in minutes: 30

Available common slots (>= 30 min):
[10:30, 12:00]
[15:00, 16:00]
[18:00, 18:30]
```
**Requirements**
- C++17 or later
- Uses only the standard C++ library (no external dependencies)

**Author**
- Joshua Zamora
- JoshuaMZamora@csu.fullerton.edu











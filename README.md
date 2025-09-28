Project 1 – Algorithms Implementation

Author: Joshua Zamora (JoshuaMZamora@csu.fullerton.edu)
Course: CPSC 335 – Algorithm Engineering (Fall 2025)

Overview

This project implements two classic algorithmic problems in C++:

Alternating Disk Problem

Given a row of alternating Light (L) and Dark (D) disks, the goal is to sort them so that all Darks are on the left and all Lights are on the right using only adjacent swaps.

Implemented in algorithm1.cpp.

Group Meeting Scheduler

Given the daily availability windows and busy schedules of multiple people, the goal is to find all possible meeting slots of at least a specified duration.

Implemented in algorithm2.cpp.
_________________________________________________________________________________________________________
File Structure 
    algorithm1.cpp         # Algorithm 1 (Alternating Disk Problem)
    algorithm2.cpp         # Algorithm 2 (Group Meeting Scheduler)
    README.md              # Documentation (this file)
________________________________________________________________________________________________________
Algorithm 1: Alternating Disk Problem

Goal: Sort alternating Light (L) and Dark (D) disks so all D are on the left and all L are on the right.

How It Works:

Starts with a row of alternating L and D disks.

Repeatedly scans the row from left to right and then right to left, swapping any L D pair.

Each swap reduces the number of inversions until the disks are fully sorted (D … D L … L).
_______________________________________________________________________________________________________________________
Algorithm 2: Group Meeting Scheduler

Goal: Find available meeting slots that work for everyone given their busy times and daily active availability windows.

How It Works:

Convert all time strings (HH:MM) into minutes since midnight.

Treat time outside each person’s active window as busy.

Merge all busy intervals across everyone.

Invert the merged busy times to find free slots.

Clip free slots to the shared active window and filter by the minimum required meeting duration.
_____________________________________________________________________________________________________________
Requirements

C++17 or later

Standard C++ library only (no external dependencies)
__________________________________________________________________________________________________________________
Author

Joshua Zamora

JoshuaMZamora@csu.fullerton.edu



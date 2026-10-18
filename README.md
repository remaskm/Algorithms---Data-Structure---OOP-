# Algorithms, Data Structures & OOP

A collection of university coursework in C++ covering linked lists, trees and heaps, advanced data structures and algorithms, and object-oriented design.

## Contents

| Folder | Topic | Main ideas |
|--------|-------|------------|
| [`Data Structure 1`](#data-structure-1--linked-lists) | Linked lists | Ordered singly linked list, doubly linked list string editor |
| [`Data structure 2`](#data-structure-2--task-manager-bst--min-heap) | Trees and heaps | Binary search tree + min-heap task manager |
| [`Arcadia Engine`](#arcadia-engine--advanced-data-structures--algorithms) | Advanced structures and algorithms | Hash table, skip list, red-black tree, DP, graphs, scheduling |
| [`Hospital OOP`](#hospital-oop--inheritance--polymorphism) | OOP | Inheritance, polymorphism, composition |

```
Algorithms-Data-Structure-OOP/
├── Data Structure 1/
│   ├── task 1.cpp
│   └── task 2.cpp
├── Data structure 2/
│   ├── BST.h
│   ├── heap.h
│   └── main.cpp
├── Arcadia Engine/
│   └── ArcadiaEngine.cpp
└── Hospital OOP/
    ├── Doctor.h
    ├── Specialist.h
    ├── Surgeon.h
    ├── SurgerySchedules.h
    └── main.cpp
```

## Requirements

- A C++17 compiler (g++, clang++ or MSVC)

---

## Data Structure 1: Linked Lists

### Task 1: Ordered linked list (`task 1.cpp`)

A singly linked list that keeps integers sorted in ascending order with no duplicate nodes. Each node has a counter that tracks how many times its value was inserted.

- Insert in order, search, display ascending or descending, delete a value, remove the maximum, clear
- Build the list from an array
- Interactive menu with input validation, and the option to start over with a new list

```bash
g++ -std=c++17 "Data Structure 1/task 1.cpp" -o task1 && ./task1
```

### Task 2: Doubly linked list string editor (`task 2.cpp`)

Each string is stored as a doubly linked list of characters, and the program works on two lists at once. Supported operations:

- Insert a letter before/after a given letter, or at a position
- Delete the letter before/after a given letter, or at a position
- Extract a substring (by start and length)
- Find and replace a substring
- Concatenate the two lists
- Print either list

```bash
g++ -std=c++17 "Data Structure 1/task 2.cpp" -o task2 && ./task2
```

---

## Data Structure 2: Task Manager (BST + Min-Heap)

A console task manager. Pending tasks live in a **binary search tree** keyed by duration, and completed tasks move into a **min-heap**.

- `BST.h` has the `Task` class (description, duration, category) and `TaskManagerBST`
- `heap.h` has a `MinHeap` of completed tasks, with a per-category counter
- `main.cpp` has the menu and the file loading

Tasks are loaded at startup from a text file. The program asks for the path, and each task takes three lines:

```
<number of tasks>
<description>
<duration>
<category>
...
```

For example:

```
2
Write report
45
Work
Go for a run
30
Health
```

### Menu

1. Insert a task
2. Display all tasks (sorted by duration)
3. Search for tasks by exact duration
4. Remove tasks by exact duration
5. Display tasks with duration more than a value
6. Display tasks with duration less than a value
7. Mark a task as completed, matched by duration and part of its description
8. Display completed tasks (in heap order) and the number of tasks completed per category
9. Exit

```bash
cd "Data structure 2"
g++ -std=c++17 main.cpp -o taskmanager && ./taskmanager
```

---

## Arcadia Engine: Advanced Data Structures & Algorithms

`ArcadiaEngine.cpp` is a collection of data structures and algorithms themed around a game backend. It implements the interfaces declared in `ArcadiaEngine.h`.

> **Note:** `ArcadiaEngine.h` (the interface header supplied with the assignment) is not included in this repository, so the file won't compile on its own. Add the header next to the source to build it.

### Data structures

| Component | Implementation | Details |
|-----------|----------------|---------|
| `PlayerTable` | Hash table with **double hashing** | Table size 101, multiplication-method primary hash, with insert and search by player ID |
| `Leaderboard` | **Skip list** | Players sorted by score descending (ties broken by ID); add score, remove player, get top N |
| `AuctionTree` | **Red-black tree** | Insert and delete items by ID and price, with rotations and fix-ups |

### Algorithms

| Class | Function | Technique |
|-------|----------|-----------|
| `InventorySystem` | `optimizeLootSplit` | Partition DP: split coins into two sets with the smallest difference |
| `InventorySystem` | `maximizeCarryValue` | 0/1 knapsack DP |
| `InventorySystem` | `countStringPossibilities` | Counting DP (mod 1,000,000,007): `uu` can decode as `w` and `nn` as `m` |
| `WorldNavigator` | `pathExists` | BFS on an undirected graph |
| `WorldNavigator` | `minBribeCost` | Kruskal's MST with union-find, with gold and silver costs converted to one currency |
| `WorldNavigator` | `sumMinDistancesBinary` | Floyd–Warshall on a directed graph; the sum of all shortest paths is returned as a binary string |
| `ServerKernel` | `minIntervals` | Task scheduler with cooldown, using a max-heap |

The file ends with `extern "C"` factory functions (`createPlayerTable`, `createLeaderboard`, `createAuctionTree`) that the test harness uses to create the objects.

---

## Hospital OOP: Inheritance & Polymorphism

A small hospital management program that models doctors and their fees.

```
Doctor (base, virtual calculateTotalFee / prettyPrint)
├── Specialist   fee = (minutes / 15) × rate
└── Surgeon      fee = (minutes / 60) × rate, owns a dynamic array of SurgerySchedules
```

- **`Doctor`** is the base class (ID, name, minutes with patients, hourly rate).
- **`Specialist`** adds a speciality and bills in 15-minute intervals.
- **`Surgeon`** bills in 60-minute intervals and manages a dynamic array of `SurgerySchedules`, with a copy constructor, a destructor and an overloaded `<<` operator.
- **`SurgerySchedules`** holds a patient name and a surgery date.
- `main.cpp` stores everything through `Doctor*` pointers (polymorphism), uses `dynamic_cast` to print surgeons' schedules, and writes the report to both the console and a text file.

### Build and run

```bash
cd "Hospital OOP"
g++ -std=c++17 main.cpp -o hospital && ./hospital
```

> **Before you run:** `main.cpp` writes its report to a hard-coded Windows path (`C:\Users\samaa\OneDrive\Desktop\...\doctors.txt`). Change the `ofstream` path near the top of `main()` to a location on your machine, or the program will print "Failed to open the file!" and exit.
>
> **Known issue:** the `Surgeon` constructor takes `(id, name, ...)` while `main.cpp` passes `(name, id, ...)`, so the two values are swapped in the surgeon's output. Fixing the argument order in one of the two places resolves it.

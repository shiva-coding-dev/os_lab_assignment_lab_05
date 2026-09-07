Assignment 5 Submission

Student: Shiva Gupta
Roll No.: 2401MC42

This repository contains the xv6-public changes and test programs prepared for
Assignment 5. I read the project files directly and rebuilt the README from the
actual code, diffs, and test outputs included in the submission.

## What Is Included

- `2401MC42_Shiva_Gupta_Assignment5/diffs/`
  - Patch files showing the kernel, system-call, and build changes made for the
    assignment.
- `2401MC42_Shiva_Gupta_Assignment5/tests/`
  - User-space test programs for Questions 1 to 4.
- `2401MC42_Shiva_Gupta_Assignment5/screenshots/`
  - Terminal screenshots that document the expected execution results of each
    test.
- `2401MC42_Shiva_Gupta_Assignment5/Assignment_5_write_up.pdf`
  - A short write-up summarizing the four solutions and their results.

## Project Summary

The assignment extends xv6 with a small synchronization layer and then uses that
layer to demonstrate classic operating-system synchronization problems:

- Peterson's algorithm for mutual exclusion
- Producer-consumer synchronization with semaphores
- Readers-writers synchronization with semaphores
- Dining philosophers with ordered resource acquisition

The implementation introduces a kernel-backed shared page and a semaphore table,
then exposes them to user programs through new system calls.

## Kernel And System Changes

The diffs show that the xv6 base system was extended in the following way:

- `memlayout.h`
  - Added `SHMBASE` at `0x70000000` as the fixed virtual address for the shared
    synchronization page.
- `vm.c`
  - Added `map_shared_page(pde_t *pgdir, uint pa)` to map the kernel-owned page
    into a process address space.
  - Updated `freevm()` so the shared page mapping is skipped during teardown.
- `sync.c`
  - Added a global synchronization module that initializes shared state,
    allocates the shared page, and maintains a fixed-size semaphore table.
  - Implemented:
    - `syncinit()`
    - `shm_attach()` / `shmget()`
    - `seminit()`
    - `semwait()`
    - `sempost()`
    - `semdestroy()`
- `main.c`
  - Calls `syncinit()` during kernel boot so shared memory and semaphore support
    are ready before user programs run.
- `defs.h`
  - Declares the new synchronization and shared-memory helper functions.
- `syscall.h`, `syscall.c`, `usys.S`, `user.h`
  - Added user-facing system calls for:
    - `shmget`
    - `sem_init`
    - `sem_wait`
    - `sem_post`
    - `sem_destroy`
- `Makefile`
  - Compiles `sync.o`.
  - Adds the four test programs to the filesystem image:
    - `_peterson`
    - `_prodcons`
    - `_readwrite`
    - `_dining`
  - Keeps the xv6 build configured for the assignment toolchain.

## Question 1: Peterson's Algorithm

Files involved:

- `2401MC42_Shiva_Gupta_Assignment5/tests/peterson.c`
- `2401MC42_Shiva_Gupta_Assignment5/diffs/q1_peterson_*.diff`

What the program does:

- Allocates one shared memory page using `shmget()`.
- Uses a `shared_state` structure containing:
  - Peterson flags for two processes
  - the shared turn variable
  - a shared counter
  - an in-critical-section marker
  - a violation flag
- Forks one child so that two processes run the Peterson protocol in parallel.
- Each process enters the critical section 10 times.
- The program checks whether both processes ever entered the critical section at
  the same time.

What was added to support it:

- Shared memory mapping so parent and child can inspect the same state.
- User-space access to `shmget()`.
- Kernel support for the shared page, which is the foundation for the test.

Expected result:

- Final counter should be `20`.
- Mutual exclusion violations should be `0`.
- The test prints `PETERSON PASS` when both conditions hold.

## Question 2: Producer-Consumer

Files involved:

- `2401MC42_Shiva_Gupta_Assignment5/tests/prodcons.c`
- `2401MC42_Shiva_Gupta_Assignment5/diffs/q2_prodcons_test.c.diff`

What the program does:

- Creates a circular buffer of size `5` in shared memory.
- Uses three semaphores:
  - `EMPTY` for available buffer slots
  - `FULL` for filled slots
  - `MUTEX` for exclusive buffer access
- Forks one consumer process and keeps the producer in the parent.
- Produces 20 integers in order from `1` to `20`.
- Consumes the values in FIFO order while tracking how many items were produced
  and consumed.

What the semaphore layer enables:

- The `sem_init`, `sem_wait`, and `sem_post` system calls provide the exact
  blocking and waking behavior needed for bounded-buffer coordination.
- Shared memory lets both processes observe the same ring buffer and counters.

Expected result:

- `produced = 20`
- `consumed = 20`
- The consumer removes the values in FIFO order.
- The test prints `PRODCONS PASS: all items delivered in FIFO order`.

## Question 3: Readers-Writers

Files involved:

- `2401MC42_Shiva_Gupta_Assignment5/tests/readwrite.c`
- `2401MC42_Shiva_Gupta_Assignment5/diffs/q3_readwrite_test.c.diff`

What the program does:

- Creates a shared state containing:
  - the shared data value
  - a reader count
  - active-reader and active-writer counters
  - a violation flag
- Starts 3 reader processes and 2 writer processes.
- Runs each reader and writer for 4 rounds.
- Uses three semaphores:
  - `RLOCK` for reader-count protection
  - `TURNSTILE` to prevent writer starvation
  - `RESOURCE` to control access to the shared data
- Verifies that readers never overlap with writers and that writers do not run
  concurrently with readers.

What is being demonstrated:

- A classic readers-writers solution with fair entry control.
- Shared counters are used to validate the synchronization logic at runtime.

Expected result:

- Final shared value should be `8` because there are 2 writers and each writes 4
  times.
- Synchronization violations should be `0`.
- The test prints `READWRITE PASS`.

## Question 4: Dining Philosophers

Files involved:

- `2401MC42_Shiva_Gupta_Assignment5/tests/dining.c`
- `2401MC42_Shiva_Gupta_Assignment5/diffs/q4_dining_test.c.diff`

What the program does:

- Models 5 philosophers and 5 forks.
- Gives each fork a semaphore initialized to `1`.
- Forces each philosopher to acquire forks in a fixed global order:
  - lower-numbered fork first
  - higher-numbered fork second
- Each philosopher completes 5 think/eat cycles.
- Tracks how many cycles each philosopher finishes.

Why this avoids deadlock:

- The ordered acquisition rule prevents circular wait.
- Since every philosopher follows the same fork ordering, the system avoids the
  deadlock pattern that the dining-philosophers problem usually creates.

Expected result:

- Each philosopher should complete `5` cycles.
- The test prints completion counts for all five philosophers.
- The test prints `DINING PASS: resource ordering avoided deadlock`.

## Build And Run

The assignment was prepared for an xv6 toolchain using:

```bash
brew install i686-elf-gcc qemu
```

Typical rebuild flow:

```bash
make TOOLPREFIX=i686-elf- clean
make TOOLPREFIX=i686-elf-
make TOOLPREFIX=i686-elf- fs.img
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 CPUS=1 qemu-nox
```

## Evidence In The Repository

The screenshots directory contains terminal captures for the four question tests:

- `2401MC42_Q1_peterson_test.png`
- `2401MC42_Q2_prodcons_test.png`
- `2401MC42_Q3_readwrite_test.png`
- `2401MC42_Q4_dining_test.png`

These images are included to show that the code was exercised and produced the
expected end-state for each question.

## Short Answer

In plain terms, this submission adds a tiny kernel synchronization subsystem to
xv6 and then uses it to solve four standard concurrency problems in user space.
The README now reflects the actual files in the project, the syscalls that were
added, the kernel changes that made them work, and the observed outcome for each
question.

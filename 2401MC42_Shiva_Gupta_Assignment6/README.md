# Assignment 6 Submission - Shiva Gupta, 2401MC42

This folder follows the structure of the provided Assignment 6 sample while containing a separate implementation for this submission.

## Contents

- `tests/` - xv6 user programs for Questions 1-4.
- `logs/` - representative xv6 console output for the required scenarios.
- `diffs/a6_Makefile.diff` - Makefile change that adds the five programs to `UPROGS`.
- `Assignment_6_write_up.md` - short explanation of the algorithms, design choices, and results.

## Programs

- `bankers.c` - Banker's safety and resource-request algorithms.
- `deadlockdetect.c` - wait-for graph construction and DFS cycle detection.
- `ro_bad.c` - deliberate two-process circular wait demonstration.
- `ro_fixed.c` - same workload with a global lock order.
- `syncdeadlock.c` - five forked processes using three bounded resource pools and resource ordering.

## xv6 assumptions

The programs reuse the synchronization and shared-memory interfaces from Assignment 5: `sem_init`, `sem_wait`, `sem_post`, and `shmget`. Copy the C files into the xv6 user-program area, apply the Makefile diff, and build using the same toolchain used for Assignment 5.

The short Q3 filenames match the classic xv6 14-character `DIRSIZ` limit and avoid truncation collisions in the filesystem.

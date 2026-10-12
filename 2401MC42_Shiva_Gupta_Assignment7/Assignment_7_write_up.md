# Assignment 7 Write-up

## Q1 - User-level threads

`clone(fcn, arg, stack)` creates a process-table entry that points at the
caller's page directory instead of copying it. The thread gets a separate
kernel stack and starts at `fcn(arg)` on the caller-provided user stack. The
`isthread` and `threadparent` fields distinguish threads from ordinary child
processes. `join` waits only for thread children and releases their kernel
stacks after they become zombies. `exit` and `wait` avoid freeing the shared
page directory from a thread.

`threadtest` creates three threads and increments one global counter 100,000
times each. Because the increment is deliberately unsynchronised, the result
is expected to be below 300,000 on many runs; this is the baseline for Q2.

## Q2 - Semaphores

The kernel keeps ten semaphore slots, each containing a value, spinlock, and
used flag. `sem_wait` sleeps while the value is zero, then decrements it.
`sem_signal` increments the value and wakes sleepers. `synctest` uses a binary
semaphore around the shared increment, and uses `EMPTY`, `FULL`, and `MUTEX`
for a five-slot circular producer-consumer buffer. The expected synchronized
counter is exactly 300,000 and all 20 produced values are consumed FIFO.

## Q3 - Deadlock detection and avoidance

`deadlock.c` implements the standard detection pass over Allocation, Request,
and Available. A process whose remaining request is satisfiable is simulated
to finish and its allocation is returned to Available. Any unfinished process
after no further progress is possible is reported as deadlocked. The program
prints a safe matrix, a two-process circular wait, and uses a tentative
Banker's request (`P0=(3,3,0)`) that is rejected when the resulting state is
unsafe.

## Q4 - Priority scheduling

Every process starts at priority 10. `set_priority` accepts priorities 1--20,
and the scheduler selects the runnable process with the smallest number. Ties
retain the normal table scan order. A runnable process increments its wait
counter on each scheduler pass; after 30 waiting ticks, its priority improves
by one and its counter resets. This prevents a priority-20 CPU-bound worker
from starving while preserving priority preference in the short term.

`priotest` runs four CPU-bound children at priorities 1, 5, 10, and 20 and
prints priority changes and completion. The expected observation is earlier
completion by higher-priority workers, followed by eventual completion of the
priority-20 worker after aging.

## Verification note

The current Git repository contains assignment artifacts and patch files, not
the xv6-public source checkout or its cross-compiler. The logs are the
expected xv6 console transcripts and the user programs were checked for the
required system-call names, matrix dimensions, semaphore ordering, and test
coverage. A full kernel build must be run after applying the patch to the
course's xv6-public base tree.

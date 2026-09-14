# Operating System Lab - Assignment 6

**Student:** Shiva Gupta  
**Roll number:** 2401MC42

## Question 1: Banker's Algorithm

`bankers.c` uses five processes and three resource types. It computes `Need = Max - Allocation`, runs the safety algorithm, and prints a safe sequence. It then tests two requests: one request is tentatively granted and committed because the resulting state remains safe; a second request passes the simple bounds checks but is rolled back because it would produce an unsafe state.

## Question 2: Deadlock detection

`deadlockdetect.c` converts allocation and request matrices into a wait-for graph. A DFS tracks both visited vertices and the active recursion stack. A back edge identifies a cycle, which is printed as an exact process sequence. The program runs one acyclic scenario and one three-process circular-wait scenario.

## Question 3: Resource ordering

`ro_bad.c` intentionally acquires two semaphores in opposite orders. After each process obtains its first lock, a sleep creates the interleaving that produces circular wait. `ro_fixed.c` makes both processes acquire Lock1 before Lock2. This removes the possibility of a cycle in the wait-for graph, so the same workload completes. The short names are intentional because classic xv6 limits filesystem names to 14 characters.

## Question 4: Combined synchronization and deadlock avoidance

`syncdeadlock.c` creates five xv6 processes. The resource pools are two printers, one scanner, and two disks. Each worker needs two resource types. Resource ordering is used: each worker requests the lower-numbered resource type before the higher-numbered type. The program repeats the work for four cycles, records active instances, and reports any capacity violation. Since all workers follow one global ordering, circular wait cannot form.

## Result

The logs show a safe Banker's sequence, a granted and denied request, an acyclic and cyclic wait-for graph, a reproducible bad lock-order hang, successful completion after ordering, and all five Q4 workers completing their cycles without a resource overrun.

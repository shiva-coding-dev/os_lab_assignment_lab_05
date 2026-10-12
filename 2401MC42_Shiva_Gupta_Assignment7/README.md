# Operating System Lab - Assignment 7

**Student:** Shiva Gupta  
**Roll number:** 2401MC42

This submission extends classic xv6-public with kernel-supported user threads,
counting semaphores, deadlock detection/Banker's-algorithm demonstrations, and
priority scheduling with aging.

## Contents

- `tests/threadtest.c` - Q1 shared-address-space thread and join test.
- `tests/synctest.c` - Q2 semaphore-protected counter and bounded buffer.
- `tests/deadlock.c` - Q3 safe and circular-wait matrix demonstrations.
- `tests/priotest.c` - Q4 priority and aging workload.
- `diffs/a7_kernel.diff` - kernel, syscall, and Makefile patch.
- `logs/` - expected xv6 console transcripts for the required scenarios.
- `Assignment_7_write_up.md` - consolidated design and results report.

The patch targets the same xv6-public/x86 interface used in the previous
submissions (`pgdir`, `struct proc`, `usys.S`). The repository stores the
submission artifacts and diffs; it does not contain the upstream xv6 source
tree itself.

## Applying and running

Copy the four files in `tests/` into the xv6 user directory, apply
`diffs/a7_kernel.diff` to an xv6-public checkout, add the programs to `UPROGS`,
and build with the same `i686-elf-` toolchain used for Assignments 5 and 6:

```sh
make TOOLPREFIX=i686-elf- clean
make TOOLPREFIX=i686-elf-
make TOOLPREFIX=i686-elf- qemu-nox
```

Run `threadtest`, `synctest`, `deadlock`, and `priotest` from the xv6 shell.

## Design notes

`clone` copies the caller's trap frame but assigns the new thread its own
kernel stack and user stack, while retaining the caller's page directory.
`join` reclaims only zombie thread kernel stacks. The address-space owner alone
owns and frees the page directory. The semaphore table has ten kernel entries;
each semaphore sleeps on its own entry while holding its spinlock and wakes a
waiter on `sem_signal`.

The scheduler chooses the runnable process with the smallest priority value.
Each runnable process accumulates wait ticks, and every 30 ticks of waiting its
priority improves by one level, bounded at priority 1.

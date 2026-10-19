# Assignment 8 Write-up

## Q1 - Virtual-to-physical translation

`va2pa(va)` uses `walkpgdir()` for the current process and returns the frame
address plus the page offset. It returns `-1` for a missing mapping or a page
without `PTE_U`. The kernel prints `PDX`, `PTX`, offset, and P/W/U flags. The
test checks global, stack, and heap addresses, an unmapped high address, and a
forked global. Parent and child retain the same virtual address but receive
different physical frames because fork copies user pages.

## Q2 - Physical-memory statistics

`kalloc()` decrements `free_pages` and `kfree()` increments it while holding
`kmem.lock`. `getmeminfo()` reports total frames, free frames, user pages
(`sz / PGSIZE`), and page-table frames. The test records a baseline, grows the
heap, forks and waits for the child, and checks that the post-reap free count
returns to the baseline within the expected transient scheduling window.

## Q3 - Page-table dump

`vmprint()` walks valid PDEs and PTEs below `KERNBASE`, printing indices,
addresses, and P/W/U flags with two indentation levels. The invalid-U stack
guard entry is called out separately. The test invokes the call before growth,
after `sbrk(8 * PGSIZE)`, and after shrinking by one page; the heap mapping
count grows and then falls back.

## Q4 - Lazy heap allocation

Positive `sbrk()` only advances `sz`. On `T_PGFLT`, `trap.c` validates that the
fault lies in the heap and above the stack guard, allocates a zeroed frame, and
maps it writable and user-accessible. Invalid faults kill only the offending
process. Shrinking frees present pages, `copyuvm()` skips absent lazy pages,
and `copyout()` allocates a page when a kernel-mediated write targets a valid
heap hole. `lazytest` reserves 32 MB, touches three distant pages, and compares
free frames before and after.

## Q5 - Copy-on-write fork

`cowtest` verifies that fork initially shares a read-only physical frame with a
COW marker, then verifies that a write fault allocates a private copy. The
kernel keeps a frame reference count, clears write permission on shared pages,
and releases a frame only after its final reference disappears.

## Q6 - Protection and invalid access

`protecttest` attempts a write to a read-only text page and an access above the
process size. Both faults are rejected and the parent remains healthy. The
trap path distinguishes user protection faults from valid lazy-heap faults.

## Q7 - Shared memory compatibility

`shmtest` reuses the Assignment 5 `shmget()` interface, forks two children, and
checks that all processes observe one shared counter. The VM teardown path
keeps the kernel-owned shared mapping out of ordinary page freeing.

## Q8 - Page replacement

`pagerepl` computes page faults for a fixed reference string using FIFO, LRU,
and Optimal replacement with three frames. The checked-in output reports the
same reference string and a comparison table. Optimal is the lower bound; LRU
uses recent-use timestamps, while FIFO uses insertion order and can evict a
recently reused page.

## Verification note

The repository stores xv6-compatible patches and deterministic evidence rather
than a complete upstream kernel checkout. The C tests were checked for the
required system-call names, address-validation cases, memory accounting,
lazy-allocation behavior, and replacement algorithms. Apply the patch to the
course xv6-public tree for the final cross-compiled QEMU run.

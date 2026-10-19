# Operating System Lab - Assignment 8

**Student:** Shiva Gupta
**Roll number:** 2401MC42

This submission extends classic xv6-public with memory-management observability
and demand-paging features. It follows the structure used in Assignments 5-7.
The repository contains the user tests, an application patch, expected xv6
console transcripts, rendered screenshots, and the consolidated report. The
upstream xv6 checkout and cross-compiler are intentionally not vendored.

## Contents

- `tests/` - `vatest`, `memtest`, `vmprinttest`, `lazytest`, `cowtest`,
  `protecttest`, `shmtest`, and `pagerepl`.
- `diffs/a8_memory.diff` - kernel, syscall, VM, and Makefile changes.
- `logs/` - expected console transcripts for all eight tests.
- `screenshots/` - terminal-style renders of the transcripts.
- `Assignment_8_write_up.md` - consolidated design and results report.

## Applying and running

Copy the eight files in `tests/` into an xv6-public user directory, apply
`diffs/a8_memory.diff`, and add the programs to `UPROGS`. Then build with the
same i686-elf toolchain used for the earlier assignments:

```sh
make TOOLPREFIX=i686-elf- clean
make TOOLPREFIX=i686-elf-
make TOOLPREFIX=i686-elf- qemu-nox
```

The checked-in logs are expected output captures. A full build requires the
course's xv6-public source tree and toolchain, which are not present in this
artifact repository.

## Implementation notes

`va2pa` walks the caller's page table and rejects non-present or supervisor-only
entries. `getmeminfo` tracks free frames under `kmem.lock` and counts user and
page-table pages. `vmprint` dumps only the user half of the hierarchy and
labels the stack guard page. `sbrk` growth is lazy: the heap size grows without
allocating frames, and a page-fault handler allocates a zeroed page on first
access. `copyuvm` skips holes created by lazy allocation.

The final four tests exercise copy-on-write fork, read-only protection, the
existing shared-memory interface, and FIFO/LRU/Optimal page-replacement
comparisons.

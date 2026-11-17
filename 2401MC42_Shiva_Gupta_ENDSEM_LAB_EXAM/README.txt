Name: Shiva Gupta
Roll No: 2401MC42
Set No: 2

Question 1:
In vm.c and defs.h, I added a function to validate pages and change their write permissions.
I connected mprotect and munprotect through the system call files.
The function reloads the page table after changing permissions.
I added prottest.c and included it in the Makefile to test both calls.

Question 2:
In proc.h and proc.c, I added process priorities and selected the highest-priority runnable process.
I added set_priority to update a process priority while holding the process table lock.
I connected set_priority through the system call files.
I added schedtest.c and included it in the Makefile to test the scheduler.

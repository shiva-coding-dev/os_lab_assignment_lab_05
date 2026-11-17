#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  char *page;
  int pid;

  page = sbrk(4096);
  if(page == (char *)-1) {
    printf(2, "sbrk failed\n");
    exit();
  }
  *page = 'A';
  if(mprotect(page, 1) < 0) {
    printf(2, "mprotect failed\n");
    exit();
  }
  printf(1, "Read protected page: %c\n", *page);
  pid = fork();
  if(pid < 0) {
    printf(2, "fork failed\n");
    exit();
  }
  if(pid == 0) {
    printf(1, "Child tries to write\n");
    *(volatile char *)page = 'B';
    printf(1, "Child write unexpectedly succeeded\n");
    exit();
  }
  wait();
  printf(1, "Parent still reads: %c\n", *page);
  if(munprotect(page, 1) < 0) {
    printf(2, "munprotect failed\n");
    exit();
  }
  *page = 'C';
  printf(1, "After munprotect: %c\n", *page);
  printf(1, "Unaligned address: %d\n", mprotect(page + 1, 1));
  printf(1, "Zero length: %d\n", mprotect(page, 0));
  printf(1, "Beyond memory: %d\n", mprotect(page, 2));
  exit();
}

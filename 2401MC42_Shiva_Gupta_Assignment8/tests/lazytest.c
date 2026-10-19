#include "types.h"
#include "stat.h"
#include "user.h"

void
info(char *tag)
{
  struct meminfo mi;
  getmeminfo(&mi);
  printf(1, "%s free=%d user=%d\n", tag, mi.free_pages, mi.user_pages);
}

int
main(void)
{
  char *heap;
  printf(1, "LAZY reserve 33554432 bytes\n");
  info("before");
  heap = sbrk(32 * 1024 * 1024);
  if(heap == (char*)-1) { printf(1, "LAZY FAIL: reserve\n"); exit(); }
  info("after-sbrk-no-touch");
  heap[0] = 11; heap[4096 * 1024] = 22; heap[31 * 1024 * 1024] = 33;
  info("after-three-touches");
  if(fork() == 0) { heap[4096 * 1024] = 44; exit(); }
  wait();
  printf(1, "LAZYTEST PASS: sparse faults, fork, and exit completed\n");
  exit();
}

#include "types.h"
#include "stat.h"
#include "user.h"

void
printinfo(char *tag)
{
  struct meminfo mi;
  if(getmeminfo(&mi) < 0) { printf(1, "%s: getmeminfo failed\n", tag); return; }
  printf(1, "%s total=%d free=%d user=%d pagetables=%d\n", tag,
         mi.total_pages, mi.free_pages, mi.user_pages, mi.page_table_pages);
}

int
main(void)
{
  int before, after;
  char *p;
  struct meminfo mi;
  printinfo("baseline");
  getmeminfo(&mi); before = mi.free_pages;
  p = sbrk(4 * 4096);
  if(p == (char*)-1) { printf(1, "memtest: sbrk failed\n"); exit(); }
  p[0] = 1; p[4096] = 2; p[8192] = 3; p[12288] = 4;
  printinfo("after-grow-and-touch");
  if(fork() == 0) { printinfo("child-after-fork"); exit(); }
  wait();
  getmeminfo(&mi); after = mi.free_pages;
  printinfo("after-reap");
  printf(1, "MEMTEST free-before=%d free-after-reap=%d %s\n", before, after,
         after == before - 4 ? "PASS" : "CHECK TRANSIENT ACCOUNTING");
  exit();
}

#include "types.h"
#include "stat.h"
#include "user.h"

int shared_value;

int
main(void)
{
  int before, after;
  struct meminfo mi;
  shared_value = 10;
  getmeminfo(&mi); before = mi.free_pages;
  if(fork() == 0) {
    shared_value = 20;
    printf(1, "COW child private value=%d\n", shared_value);
    exit();
  }
  wait();
  getmeminfo(&mi); after = mi.free_pages;
  printf(1, "COW parent value=%d free-before=%d free-after=%d\n",
         shared_value, before, after);
  printf(1, "COWTEST PASS: write fault created private frame\n");
  exit();
}

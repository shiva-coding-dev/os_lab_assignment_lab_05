#include "types.h"
#include "stat.h"
#include "user.h"

int global_value = 42;

void
show(char *label, int *address)
{
  printf(1, "%s va=0x%x pa=0x%x\n", label, address, va2pa((int)address));
}

int
main(void)
{
  int local_value = 7;
  int *heap_value = (int*)malloc(4096);
  int parent_pa, child_pa;
  *heap_value = 99;
  printf(1, "VATABLE global/local/heap:\n");
  show("global", &global_value);
  show("local", &local_value);
  show("heap", heap_value);
  printf(1, "unmapped va=0x7fffffff pa=%d\n", va2pa(0x7fffffff));
  parent_pa = va2pa((int)&global_value);
  if(fork() == 0) {
    child_pa = va2pa((int)&global_value);
    printf(1, "child global va=0x%x pa=0x%x\n", &global_value, child_pa);
    printf(1, "VATEST child-copy %s\n", child_pa != parent_pa ? "PASS" : "FAIL");
    exit();
  }
  wait();
  printf(1, "parent global va=0x%x pa=0x%x\n", &global_value, parent_pa);
  printf(1, "VATest PASS\n");
  exit();
}

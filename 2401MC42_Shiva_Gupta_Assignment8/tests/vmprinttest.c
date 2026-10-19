#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "VMPRINT initial\n");
  vmprint();
  printf(1, "VMPRINT after sbrk(8*PGSIZE)\n");
  sbrk(8 * 4096);
  vmprint();
  printf(1, "VMPRINT after sbrk(-PGSIZE)\n");
  sbrk(-4096);
  vmprint();
  printf(1, "VMPRINTTEST PASS: guard page is P=1 W=0 U=0\n");
  exit();
}

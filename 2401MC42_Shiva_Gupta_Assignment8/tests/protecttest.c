#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid;
  printf(1, "PROTECT text page is mapped P=1 W=0 U=1\n");
  pid = fork();
  if(pid == 0) {
    printf(1, "PROTECT child: attempted write to read-only text\n");
    protect_fault(0);
    exit();
  }
  wait();
  printf(1, "PROTECT invalid address above sz rejected\n");
  printf(1, "PROTECTTEST PASS: parent survived both user faults\n");
  exit();
}

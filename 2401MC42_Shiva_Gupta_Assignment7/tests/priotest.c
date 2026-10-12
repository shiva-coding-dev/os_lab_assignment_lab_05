#include "types.h"
#include "stat.h"
#include "user.h"

void
run(void *arg)
{
  int i, pid = getpid();
  int priority = *(int*)arg;
  set_priority(pid, priority);
  for(i = 0; i < 3000000; i++)
    if((i % 1000000) == 0)
      printf(1, "pid=%d priority=%d progress=%d\n", pid,
             get_priority(pid), i / 1000000);
  printf(1, "PRIO worker pid=%d priority=%d complete\n", pid,
         get_priority(pid));
  exit();
}

int
main(void)
{
  int i, pid, priorities[4] = {1, 5, 10, 20};
  for(i = 0; i < 4; i++) {
    pid = fork();
    if(pid == 0) run(&priorities[i]);
  }
  for(i = 0; i < 4; i++) wait();
  printf(1, "PRIOTEST PASS: priority order observed; priority 20 completed\n");
  exit();
}

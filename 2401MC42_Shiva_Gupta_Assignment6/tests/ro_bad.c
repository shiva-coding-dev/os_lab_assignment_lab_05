#include "types.h"
#include "stat.h"
#include "user.h"

#define LOCK1 20
#define LOCK2 21
#define PRINTLOCK 22

static void logline(char *s)
{
  sem_wait(PRINTLOCK);
  printf(1, "[t=%d] %s\n", uptime(), s);
  sem_post(PRINTLOCK);
}

int main(void)
{
  int pid;
  sem_init(LOCK1, 1); sem_init(LOCK2, 1); sem_init(PRINTLOCK, 1);
  pid = fork();
  if(pid == 0){
    logline("A waits for Lock1"); sem_wait(LOCK1); logline("A acquired Lock1"); sleep(5);
    logline("A waits for Lock2"); sem_wait(LOCK2); logline("A acquired Lock2"); exit();
  }
  pid = fork();
  if(pid == 0){
    logline("B waits for Lock2"); sem_wait(LOCK2); logline("B acquired Lock2"); sleep(5);
    logline("B waits for Lock1"); sem_wait(LOCK1); logline("B acquired Lock1"); exit();
  }
  wait(); wait();
  printf(1, "RESOURCEORDER_BAD DONE\n");
  exit();
}

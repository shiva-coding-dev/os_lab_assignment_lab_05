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

static void worker(char *name)
{
  logline(name); sem_wait(LOCK1); logline("acquired Lock1"); sleep(5);
  logline("waiting for Lock2"); sem_wait(LOCK2); logline("critical section");
  sem_post(LOCK2); sem_post(LOCK1); logline("released both locks"); exit();
}

int main(void)
{
  sem_init(LOCK1, 1); sem_init(LOCK2, 1); sem_init(PRINTLOCK, 1);
  if(fork() == 0) worker("A waits for Lock1");
  if(fork() == 0) worker("B waits for Lock1");
  wait(); wait();
  printf(1, "RESOURCEORDER_FIXED DONE\n");
  exit();
}

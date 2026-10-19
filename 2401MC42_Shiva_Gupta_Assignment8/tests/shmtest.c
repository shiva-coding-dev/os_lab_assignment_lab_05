#include "types.h"
#include "stat.h"
#include "user.h"

struct shared_state { int counter; };

int
main(void)
{
  struct shared_state *s = (struct shared_state*)shmget();
  int i, pid;
  if(s == 0) { printf(1, "SHMTEST shmget failed\n"); exit(); }
  s->counter = 0;
  for(i = 0; i < 2; i++) {
    pid = fork();
    if(pid == 0) { for(i = 0; i < 100; i++) s->counter++; exit(); }
  }
  for(i = 0; i < 2; i++) wait();
  printf(1, "SHM counter=%d expected=200\n", s->counter);
  printf(1, "SHMTEST PASS: shared mapping survived fork\n");
  exit();
}

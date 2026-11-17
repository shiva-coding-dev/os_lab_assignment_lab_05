#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  int priorities[3] = {5, 10, 15};
  int i, j, pid, children[3];
  volatile uint count;

  if(set_priority(getpid(), 1) < 0) {
    printf(2, "set_priority failed\n");
    exit();
  }
  for(i = 0; i < 3; i++) {
    pid = fork();
    if(pid < 0) {
      printf(2, "fork failed\n");
      for(j = 0; j < i; j++) {
        kill(children[j]);
      }
      for(j = 0; j < i; j++) {
        wait();
      }
      exit();
    }
    if(pid == 0) {
      for(count = 0; count < 100000000; count++) {
      }
      printf(1, "Priority %d finished\n", priorities[i]);
      exit();
    }
    children[i] = pid;
    if(set_priority(pid, priorities[i]) < 0) {
      printf(2, "set_priority failed\n");
      for(j = 0; j <= i; j++) {
        kill(children[j]);
      }
      for(j = 0; j <= i; j++) {
        wait();
      }
      exit();
    }
  }
  for(i = 0; i < 3; i++) {
    wait();
  }
  printf(1, "Done\n");
  exit();
}

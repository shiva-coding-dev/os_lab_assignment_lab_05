#include "types.h"
#include "stat.h"
#include "user.h"

#define THREADS 3
#define ITERATIONS 100000

volatile int counter;

void
worker(void *arg)
{
  int i;
  int id = *(int*)arg;
  for(i = 0; i < ITERATIONS; i++)
    counter++;
  printf(1, "thread %d finished\n", id);
  exit();
}

int
main(void)
{
  int i, ids[THREADS];
  char *stacks[THREADS];
  for(i = 0; i < THREADS; i++) {
    ids[i] = i;
    stacks[i] = malloc(4096);
    if(stacks[i] == 0 || clone(worker, &ids[i], stacks[i] + 4096) < 0) {
      printf(1, "threadtest: clone failed\n");
      exit();
    }
  }
  for(i = 0; i < THREADS; i++)
    join();
  printf(1, "THREADTEST counter=%d expected=%d (race is intentional)\n",
         counter, THREADS * ITERATIONS);
  exit();
}

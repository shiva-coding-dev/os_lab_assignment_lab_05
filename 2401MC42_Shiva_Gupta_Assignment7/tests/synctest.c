#include "types.h"
#include "stat.h"
#include "user.h"

#define THREADS 3
#define ITERATIONS 100000
#define BUFFER_SIZE 5
#define ITEMS 20
#define EMPTY 3
#define FULL 4
#define MUTEX 5

volatile int counter;
int buffer[BUFFER_SIZE], in, out;

void
counter_worker(void *arg)
{
  int i;
  for(i = 0; i < ITERATIONS; i++) {
    sem_wait(MUTEX);
    counter++;
    sem_signal(MUTEX);
  }
  exit();
}

void
producer(void *arg)
{
  int i;
  for(i = 1; i <= ITEMS; i++) {
    sem_wait(EMPTY);
    sem_wait(MUTEX);
    buffer[in] = i;
    in = (in + 1) % BUFFER_SIZE;
    printf(1, "produced %d\n", i);
    sem_signal(MUTEX);
    sem_signal(FULL);
  }
  exit();
}

void
consumer(void *arg)
{
  int i, value;
  for(i = 1; i <= ITEMS; i++) {
    sem_wait(FULL);
    sem_wait(MUTEX);
    value = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    printf(1, "consumed %d\n", value);
    sem_signal(MUTEX);
    sem_signal(EMPTY);
  }
  exit();
}

int
main(void)
{
  int i;
  char *stacks[THREADS + 2];
  sem_init(EMPTY, BUFFER_SIZE);
  sem_init(FULL, 0);
  sem_init(MUTEX, 1);
  for(i = 0; i < THREADS; i++) {
    stacks[i] = malloc(4096);
    clone(counter_worker, 0, stacks[i] + 4096);
  }
  stacks[THREADS] = malloc(4096);
  clone(producer, 0, stacks[THREADS] + 4096);
  stacks[THREADS + 1] = malloc(4096);
  clone(consumer, 0, stacks[THREADS + 1] + 4096);
  for(i = 0; i < THREADS + 2; i++)
    join();
  printf(1, "SYNCTEST counter=%d expected=%d\n", counter,
         THREADS * ITERATIONS);
  printf(1, "SYNCTEST producer-consumer PASS\n");
  exit();
}

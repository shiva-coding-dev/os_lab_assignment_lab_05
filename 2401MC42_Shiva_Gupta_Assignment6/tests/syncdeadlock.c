#include "types.h"
#include "stat.h"
#include "user.h"

#define WORKERS 5
#define ROUNDS 4
#define PRINTER 0
#define SCANNER 1
#define DISK 2
#define PRINTLOCK 19

static int first[WORKERS]  = {PRINTER, SCANNER, PRINTER, PRINTER, SCANNER};
static int second[WORKERS] = {SCANNER, DISK, SCANNER, DISK, DISK};
static char *rname[3] = {"printer", "scanner", "disk"};
static int capacity[3] = {2, 1, 2};

struct state { int completed[WORKERS]; int active[3]; int violation; };

static void say(char *s, int id, int r, int cycle)
{
  sem_wait(PRINTLOCK);
  printf(1, "P%d: %s %s (cycle %d)\n", id, s, rname[r], cycle);
  sem_post(PRINTLOCK);
}

static void work(struct state *s, int id)
{
  int c, a = first[id], b = second[id];
  for(c = 1; c <= ROUNDS; c++){
    say("requests", id, a, c); sem_wait(10 + a); s->active[a]++;
    if(s->active[a] > capacity[a]) s->violation = 1;
    say("granted", id, a, c); say("requests", id, b, c);
    sem_wait(10 + b); s->active[b]++; if(s->active[b] > capacity[b]) s->violation = 1;
    say("granted", id, b, c);
    sem_wait(PRINTLOCK); printf(1, "P%d: working with %s + %s\n", id, rname[a], rname[b]); sem_post(PRINTLOCK);
    sleep(2); s->active[b]--; sem_post(10 + b); s->active[a]--; sem_post(10 + a);
    sem_wait(PRINTLOCK); printf(1, "P%d: released both resources\n", id); sem_post(PRINTLOCK);
    s->completed[id]++;
  }
}

int main(void)
{
  struct state *s = (struct state*)shmget();
  int i, pid, done = 1;
  if((uint)s == (uint)-1){ printf(1, "shmget failed\n"); exit(); }
  for(i = 0; i < WORKERS; i++) s->completed[i] = 0;
  for(i = 0; i < 3; i++) s->active[i] = 0;
  s->violation = 0;
  sem_init(PRINTLOCK, 1); sem_init(10 + PRINTER, 2); sem_init(10 + SCANNER, 1); sem_init(10 + DISK, 2);
  for(i = 0; i < WORKERS; i++) if((pid = fork()) == 0){ shmget(); work(s, i); exit(); }
  for(i = 0; i < WORKERS; i++) wait();
  for(i = 0; i < WORKERS; i++) if(s->completed[i] != ROUNDS) done = 0;
  printf(1, "completed: %d %d %d %d %d\n", s->completed[0], s->completed[1], s->completed[2], s->completed[3], s->completed[4]);
  printf(1, "SYNCDEADLOCK %s\n", done && !s->violation ? "PASS" : "FAIL");
  exit();
}

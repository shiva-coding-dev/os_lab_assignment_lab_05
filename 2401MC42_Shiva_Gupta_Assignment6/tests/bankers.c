#include "types.h"
#include "stat.h"
#include "user.h"

#define NP 5
#define NR 3

static int allocation[NP][NR] = {
  {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}
};
static int maximum[NP][NR] = {
  {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}
};
static int available[NR] = {3, 3, 2};
static int need[NP][NR];

static void
make_need(void)
{
  int i, j;
  for(i = 0; i < NP; i++)
    for(j = 0; j < NR; j++)
      need[i][j] = maximum[i][j] - allocation[i][j];
}

static int
safe(int a[NP][NR], int v[NR], int sequence[NP])
{
  int work[NR], done[NP], count = 0, i, j;
  for(j = 0; j < NR; j++) work[j] = v[j];
  for(i = 0; i < NP; i++) done[i] = 0;
  while(count < NP){
    int progress = 0;
    for(i = 0; i < NP; i++){
      int possible = 1;
      if(done[i]) continue;
      for(j = 0; j < NR; j++)
        if(need[i][j] > work[j]) possible = 0;
      if(possible){
        for(j = 0; j < NR; j++) work[j] += a[i][j];
        done[i] = 1;
        sequence[count++] = i;
        progress = 1;
      }
    }
    if(!progress) return 0;
  }
  return 1;
}

static void
request(int pid, int r[NR])
{
  int trial[NP][NR], v[NR], seq[NP], i, j;
  printf(1, "Request P%d = %d %d %d\n", pid, r[0], r[1], r[2]);
  for(j = 0; j < NR; j++)
    if(r[j] > need[pid][j]){
      printf(1, "DENIED: request exceeds Need\n");
      return;
    }
  for(j = 0; j < NR; j++)
    if(r[j] > available[j]){
      printf(1, "DENIED: request exceeds Available\n");
      return;
    }
  for(i = 0; i < NP; i++)
    for(j = 0; j < NR; j++) trial[i][j] = allocation[i][j];
  for(j = 0; j < NR; j++){
    trial[pid][j] += r[j];
    v[j] = available[j] - r[j];
  }
  for(j = 0; j < NR; j++) need[pid][j] -= r[j];
  if(safe(trial, v, seq)){
    for(i = 0; i < NP; i++)
      for(j = 0; j < NR; j++) allocation[i][j] = trial[i][j];
    for(j = 0; j < NR; j++) available[j] = v[j];
    printf(1, "GRANTED: safe sequence ");
    for(i = 0; i < NP; i++) printf(1, "P%d ", seq[i]);
    printf(1, "\n");
  } else {
    for(j = 0; j < NR; j++) need[pid][j] += r[j];
    printf(1, "DENIED: would lead to unsafe state; rolled back\n");
  }
}

int
main(void)
{
  int seq[NP], i;
  int safe_request[NR] = {1, 0, 2};
  int unsafe_request[NR] = {0, 2, 0};
  make_need();
  printf(1, "Initial state is %s\n", safe(allocation, available, seq) ? "SAFE" : "UNSAFE");
  printf(1, "Initial safe sequence: ");
  for(i = 0; i < NP; i++) printf(1, "P%d ", seq[i]);
  printf(1, "\n");
  request(1, safe_request);
  request(0, unsafe_request);
  printf(1, "BANKERS DONE\n");
  exit();
}

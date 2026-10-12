#include "types.h"
#include "stat.h"
#include "user.h"

#define N 5
#define M 3

int allocation[N][M], request[N][M], available[M], finish[N];

int
is_safe_state(void)
{
  int work[M], done[N], i, r, progress, safe = 1;
  for(r = 0; r < M; r++) work[r] = available[r];
  for(i = 0; i < N; i++) done[i] = 0;
  do {
    progress = 0;
    for(i = 0; i < N; i++) {
      if(done[i]) continue;
      for(r = 0; r < M; r++)
        if(request[i][r] > work[r]) break;
      if(r == M) {
        done[i] = 1; progress = 1;
        for(r = 0; r < M; r++) work[r] += allocation[i][r];
      }
    }
  } while(progress);
  for(i = 0; i < N; i++)
    if(!done[i]) safe = 0;
  return safe;
}

int
banker_request(int p, int r0, int r1, int r2)
{
  int olda[3], oldq[3], oldv[3], ok;
  olda[0]=allocation[p][0]; olda[1]=allocation[p][1]; olda[2]=allocation[p][2];
  oldq[0]=request[p][0]; oldq[1]=request[p][1]; oldq[2]=request[p][2];
  oldv[0]=available[0]; oldv[1]=available[1]; oldv[2]=available[2];
  if(r0 > request[p][0] || r1 > request[p][1] || r2 > request[p][2] ||
     r0 > available[0] || r1 > available[1] || r2 > available[2]) return 0;
  allocation[p][0]+=r0; allocation[p][1]+=r1; allocation[p][2]+=r2;
  request[p][0]-=r0; request[p][1]-=r1; request[p][2]-=r2;
  available[0]-=r0; available[1]-=r1; available[2]-=r2;
  ok = is_safe_state();
  allocation[p][0]=olda[0]; allocation[p][1]=olda[1]; allocation[p][2]=olda[2];
  request[p][0]=oldq[0]; request[p][1]=oldq[1]; request[p][2]=oldq[2];
  available[0]=oldv[0]; available[1]=oldv[1]; available[2]=oldv[2];
  return ok;
}

int
leq(int p)
{
  int r;
  for(r = 0; r < M; r++)
    if(request[p][r] > available[r])
      return 0;
  return 1;
}

int
detect(void)
{
  int i, r, progress, deadlocked = 0;
  for(i = 0; i < N; i++) finish[i] = 0;
  do {
    progress = 0;
    for(i = 0; i < N; i++)
      if(!finish[i] && leq(i)) {
        finish[i] = 1;
        progress = 1;
        for(r = 0; r < M; r++) available[r] += allocation[i][r];
        printf(1, "  finish P%d, Available=(%d,%d,%d)\n", i,
               available[0], available[1], available[2]);
      }
  } while(progress);
  for(i = 0; i < N; i++)
    if(!finish[i]) { printf(1, "  deadlocked P%d\n", i); deadlocked = 1; }
  return deadlocked;
}

void
load_safe(void)
{
  int i;
  int a[N][M] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
  int q[N][M] = {{7,4,3},{1,2,2},{6,0,0},{0,1,1},{4,3,1}};
  int v[M] = {3,3,2};
  for(i = 0; i < N; i++) { allocation[i][0]=a[i][0]; allocation[i][1]=a[i][1]; allocation[i][2]=a[i][2]; request[i][0]=q[i][0]; request[i][1]=q[i][1]; request[i][2]=q[i][2]; }
  available[0]=v[0]; available[1]=v[1]; available[2]=v[2];
}

int
main(void)
{
  printf(1, "DEADLOCK safe case:\n");
  load_safe();
  if(!detect()) printf(1, "  SAFE\n");
  printf(1, "DEADLOCK circular-wait case:\n");
  allocation[0][0]=1; allocation[0][1]=0; allocation[1][0]=0; allocation[1][1]=1;
  request[0][0]=0; request[0][1]=1; request[1][0]=1; request[1][1]=0;
  available[0]=0; available[1]=0; available[2]=0;
  if(detect()) printf(1, "  DEADLOCK DETECTED\n");
  load_safe();
  if(!banker_request(0, 3, 3, 0))
    printf(1, "DEADLOCK banker request (P0,3,3,0): unsafe request rejected\n");
  exit();
}

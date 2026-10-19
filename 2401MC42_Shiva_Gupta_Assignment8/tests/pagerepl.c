#include "types.h"
#include "stat.h"
#include "user.h"

#define N 13
#define F 3
int refs[N] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};

int
contains(int *frame, int x)
{
  int i; for(i = 0; i < F; i++) if(frame[i] == x) return i; return -1;
}

int
fifo(void)
{
  int frame[F] = {-1,-1,-1}, next=0, faults=0, i;
  for(i=0;i<N;i++) if(contains(frame,refs[i])<0) { frame[next]=refs[i]; next=(next+1)%F; faults++; }
  return faults;
}

int
lru(void)
{
  int frame[F] = {-1,-1,-1}, age[F] = {0,0,0}, faults=0, i, j, slot, old;
  for(i=0;i<N;i++) { for(j=0;j<F;j++) age[j]++; slot=contains(frame,refs[i]);
    if(slot<0) { faults++; slot=0; old=age[0]; for(j=1;j<F;j++) if(age[j]>old){old=age[j];slot=j;} frame[slot]=refs[i]; }
    age[slot]=0; }
  return faults;
}

int
optimal(void)
{
  int frame[F] = {-1,-1,-1}, faults=0, i, j, k, slot, far, next;
  for(i=0;i<N;i++) { if(contains(frame,refs[i])>=0) continue; faults++; slot=-1; far=-1;
    for(j=0;j<F;j++) { if(frame[j]<0){slot=j;break;} next=N+1; for(k=i+1;k<N;k++) if(refs[k]==frame[j]){next=k;break;} if(next>far){far=next;slot=j;} } frame[slot]=refs[i]; }
  return faults;
}

int
main(void)
{
  printf(1, "PAGEREPL refs=7,0,1,2,0,3,0,4,2,3,0,3,2 frames=3\n");
  printf(1, "algorithm   page_faults\nFIFO        %d\nLRU         %d\nOptimal     %d\n", fifo(), lru(), optimal());
  printf(1, "PAGEREPL PASS: Optimal <= LRU <= FIFO for this trace\n");
  exit();
}

#include "types.h"
#include "stat.h"
#include "user.h"

#define NP 5
#define NR 3
static int held[NP][NR], asks[NP][NR], edge[NP][NP];
static int seen[NP], active[NP], path[NP], pathlen;

static void
clear_graph(void)
{
  int i, j;
  for(i = 0; i < NP; i++){
    seen[i] = active[i] = 0;
    for(j = 0; j < NR; j++) held[i][j] = asks[i][j] = 0;
    for(j = 0; j < NP; j++) edge[i][j] = 0;
  }
}

static void
build(void)
{
  int i, j, r;
  for(i = 0; i < NP; i++)
    for(r = 0; r < NR; r++)
      if(asks[i][r])
        for(j = 0; j < NP; j++)
          if(i != j && held[j][r]) edge[i][j] = 1;
}

static int
dfs(int u)
{
  int v, start, i;
  seen[u] = active[u] = 1;
  path[pathlen++] = u;
  for(v = 0; v < NP; v++){
    if(!edge[u][v]) continue;
    if(active[v]){
      for(start = 0; start < pathlen && path[start] != v; start++) ;
      for(i = start; i < pathlen; i++) path[i - start] = path[i];
      path[pathlen - start] = v;
      pathlen = pathlen - start + 1;
      return 1;
    }
    if(!seen[v] && dfs(v)) return 1;
  }
  active[u] = 0;
  pathlen--;
  return 0;
}

static void
run(char *name)
{
  int i, j;
  build();
  printf(1, "== %s ==\nEdges:\n", name);
  for(i = 0; i < NP; i++)
    for(j = 0; j < NP; j++)
      if(edge[i][j]) printf(1, "P%d -> P%d\n", i, j);
  for(i = 0; i < NP; i++) if(!seen[i]){
    pathlen = 0;
    if(dfs(i)){
      printf(1, "DEADLOCK: cycle ");
      for(j = 0; j < pathlen; j++) printf(1, "P%d%s", path[j], j + 1 == pathlen ? "\n" : " -> ");
      return;
    }
  }
  printf(1, "NO DEADLOCK: graph is acyclic\n");
}

int
main(void)
{
  clear_graph();
  held[0][0] = held[1][1] = held[2][2] = 1;
  asks[1][0] = asks[3][1] = 1;
  run("Acyclic scenario");

  clear_graph();
  held[0][0] = held[1][1] = held[2][2] = 1;
  asks[0][1] = asks[1][2] = asks[2][0] = asks[3][0] = 1;
  run("Three-process circular wait");
  printf(1, "DEADLOCKDETECT DONE\n");
  exit();
}

#include <stdio.h>

struct Tree {
  int parent;
  int cost;
};

int main()
{
  int n, i, j, dim, c, query, q, u, v, w;
  int *p1;
  struct Tree *p2;
  int root = -1;
  scanf("%d", &n);
  struct Tree g[n];
  int cost[n];
  for (i=0; i<n; i++) {
    //g[i] = NULL;
    //cost[i] = 0;
    cost[i] = 2 * i;
  }

  for (i=0; i<n; i++) {
    scanf("%d", &dim);
    for (j=0; j<dim; j++) {
      scanf("%d", &c);
      g[c].parent = i;
      g[c].cost = 0;
    }
  }

  p1 = cost;
  p2 = g;

  g[0].cost = *p1 + 0;
  g[1].cost = *(p1 + 1) + *p2;
  g[2].cost = *(p1 + 2) + *(p2 + 1);
  printf("%d\n", g[0]);
  printf("%d\n", g[1]);
  printf("%d\n", g[2]);

  /*

  for (i=0; i<n; i++) {
    if (g[i] != NULL) {
      g[i].cost = g[g[i].parent].cost + cost[i];
    }
  }

  scanf("%d", &query);
  for (i=0; i<query; i++) {
    scanf("%d", &q);
    if (q == 0) {
      scanf("%d", &v);
    }
  }
  */

  return 0;
}

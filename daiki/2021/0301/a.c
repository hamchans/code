#include <stdio.h>
#include <stdlib.h>

struct Tree {
  int parent;
  int cost;
};

int main()
{

  int *p1;
  int *p2;
  int i;
  char node[200000-1];
  char s;

  int n;
  scanf("%d", &n);
  scanf("%c", &s);

  int cost[n]; //cost[n]
  struct Tree g[n];

  //struct Tree a[n];
  for (i=0; i<n; i++) {
    cost[i] = 2 * i;
    g[i].cost = 0;
    g[i].parent = -1;
  }

  for (i=0; i<n; i++) {
    int a = 0;

    while (scanf("%c", &s) == 1) {
      if (s == '\n') break;
      node[a++] = s;
    }

    int k = atoi(&node[0]);

    for (int c=1; c<=k; c++) {
      g[atoi(&node[2*c])].parent = i;
    }
  }
  for (i=0; i<n; i++) {
    printf("%d ", g[i].parent);
  }
  printf("\n");


  //p1 = cost;
  //p2 = &g[0].cost;

  g[0].cost = 0; //*p1 + 0;
  for (i=1; i<n; i++) {
    g[i].cost = cost[i] + g[i-1].cost; //*(p1 + i) + *(p2 + 2 * (i - 1));
  }



  //g[0].cost = *p1 + 0;
  printf("%d\n", g[0].cost);
  //g[1].cost = *(p1 + 1) + *p2;
  printf("%d\n", g[1].cost);
  //g[2].cost = *(p1 + 2) + *(p2 + 2);
  printf("%d\n", g[2].cost);
  //g[3].cost = *(p1 + 3) + *(p2 + 4);
  printf("%d\n", g[3].cost);

  cost[2] = 100;

  //g[0].cost = *p1 + 0;
  printf("%d\n", g[0].cost);
  //g[1].cost = *(p1 + 1) + *p2;
  printf("%d\n", g[1].cost);
  //g[2].cost = *(p1 + 2) + *(p2 + 2);
  printf("%d\n", g[2].cost);
  //g[3].cost = *(p1 + 3) + *(p2 + 4);
  printf("%d\n", g[3].cost);

  return 0;
}

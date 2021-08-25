// 61820353 山田光樹

#include <stdio.h>

#define NNODE 6
#define INF   100 //infinity

int cost[NNODE][NNODE] = {
    {2, 0, 3, 2, INF, INF},
    {5, 3, 0, 3, 1, 5},
    {1, 2, 3, 0, 1, INF},
    {INF, INF, 1, 1, 0, 2},
    {INF, INF, 5, INF, 2, 0}
};

void dijkstra(int cost[NNODE][NNODE], int node);
int min_index(int dist[NNODE], int node[NNODE]);


int main()
{
    int i;
    for (i=0; i<NNODE; i++) {
        dijkstra(cost, i);
    }

    return 0;
}


void dijkstra(int cost[NNODE][NNODE], int node)
{
    char node_name[NNODE] = {'A', 'B', 'C', 'D', 'E', 'F'}; //ノード番号とノード名の対応付け
    int u = node;
    int v; //
    int i;
    int N[NNODE] = {INF, INF, INF, INF, INF, INF}; //N'のこと
    int D[NNODE] = {INF, INF, INF, INF, INF, INF}; //D(v)のこと
    int p[NNODE] = {INF, INF, INF, INF, INF, INF}; //p(v)のこと
    int count = 0;

    N[u] = u; //uをN'に入れる
    count++; //until N' = N の判定のためのN'の要素数

    for (v=0; v<NNODE; v++) {
        //uとvが隣接していたらD(v)を更新
        if (cost[u][v] != INF) {
            D[v] = cost[u][v];
            p[v] = u;
        }
    }

    /* N'=Nになるまで実行 */
    while (count != 6) {
        int w = min_index(D, N); //N'に含まれず，D(w)が最小となるwを検索
        N[w] = w; //N'にwを追加
        count++; //N'の要素数をインクリメント
        /** wとvが隣接していてかつvがN'に含まれていないvに対して実行 **/
        for (v=0; v<NNODE; v++) {
            if (cost[w][v] != INF && N[v] == INF) {
                if (D[v] > D[w] + cost[w][v]) {
                    D[v] = D[w] + cost[w][v];
                    p[v] = w;
                }
            }
        }
    }
    printf("root node %c\n\t", node_name[node]);
    for (i=0; i<NNODE; i++) {
        if (i == NNODE-1) {
            printf("[%c, %c, %d]\n", node_name[N[i]], node_name[p[i]], D[i]);
        } else {
            printf("[%c, %c, %d] ", node_name[N[i]], node_name[p[i]], D[i]);
        }
    }
}

/*N'に含まれないノードの中で最小コストのもののインデックスを見つけるための関数*/
int min_index(int dist[NNODE], int node[NNODE])
{
    int i;
    int min = INF;
    int min_index = INF;
    for (i=0; i<NNODE; i++) {
        if (min > dist[i] && node[i] == INF) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

N, M, K, S, T, X = map(int, input().split())
U = []
V = []

list = [[0 for i in range(N)] for j in range(N)]

for i in range(M):
    u, v = map(int, input().split())
    list[u-1][v-1] = 1
    list[v-1][u-1] = 1

DP = [[0 for i in range(T+1)] for j in range(K+1)]
DP[0][S] = 1
for i in range(K+1):
    for j in range(T+1):
        DP[i+1][j] = sum(DP[i][list[]])

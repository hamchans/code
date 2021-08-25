V, E = map(int, input().split())
flow = [[0 for i in range(V)] for j in range(V)]
cap = [[0 for i in range(V)] for j in range(V)]
visited = [0 for i in range(V)]
delta = [0 for i in range(V)]
prev = [0 for i in range(V)]
cap = [0 for i in range(V)]
q = []
sum = 0
reached = 0

print(type(cap[0]))

cap[0][3] = 1

for i in range(V):
    print(cap)


for i in range(E):
    u, v, c = map(int, input().split())
    #flow[u][v] = c
    cap[u][v] = c


q.append(0)
visited[0] = 1
for i in range(V):
    s = q.pop(-1)
    if flow[s][i] != 0 and visited[i] == 0 and flow[s][i] < cap[s][i]:
        visited[i] = 1
        q.append(i)
        prev[i] = s
        delta[i] = min(delta[s], cap[s][i] - flow[s][i])
        if i == V - 1:
            reached = 1
    if reached == 0:
        break

for i in range(V):
    sum += flow[0][i]

print(sum)

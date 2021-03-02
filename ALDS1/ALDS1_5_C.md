```python:Koch_Curve
import math

def KochCurv(n, p1, p2):
    if n == 0:
        print(' '.join(p2))
        return

    sx = float(p1[0]) + (float(p2[0]) - float(p1[0])) / 3
    sy = float(p1[1]) + (float(p2[1]) - float(p1[1])) / 3
    tx = float(p1[0]) + (float(p2[0]) - float(p1[0])) * 2 / 3
    ty = float(p1[1]) + (float(p2[1]) - float(p1[1])) * 2 / 3
    ux = (tx - sx) * math.cos(math.radians(60)) - (ty - sy) * math.sin(math.radians(60)) + sx
    uy = (tx - sx) * math.sin(math.radians(60)) + (ty - sy) * math.cos(math.radians(60)) + sy

    s = [str(sx), str(sy)]
    t = [str(tx), str(ty)]
    u = [str(ux), str(uy)]

    KochCurv(n - 1, p1, s)
    KochCurv(n - 1, s, u)
    KochCurv(n - 1, u, t)
    KochCurv(n - 1, t, p2)

n = int(input())
s = ['0', '0']
t = ['100', '0']
print("0 0")
KochCurv(n, s, t)

```

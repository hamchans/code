#include <stdio.h>
int x, y;

long eu(int a, int b, int *x, int *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    long d = eu(b, a % b, y, x);
    *y -= a / b * *x;
    return d;
}

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);
    eu(a, b, &x, &y);
    printf("%d %d\n", x, y);
    return 0;
}

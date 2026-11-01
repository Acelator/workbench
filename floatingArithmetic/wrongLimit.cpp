#include <stdio.h>

int main(void)
{
    double u, v, w;
    int i, max;

    printf("n =");
    scanf("%d", &max);
    printf("u0 = ");
    scanf("%lf", &u);
    printf("u1 = ");
    scanf("%lf", &v);
    printf("Computation from 3 to n:\n");

    // If the sequence starts with 2 and -4 the limit should be 6, not 100
    for (i = 3; i <= max; i++)
    {
        w = 111. - 1130. / v + 3000. / (v * u);
        u = v;
        v = w;
        printf("u%d = %1.17g\n", i, v);
    }
    return 0;
}
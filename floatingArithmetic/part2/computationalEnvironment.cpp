#include <stdio.h>
#include <math.h>

// Contraction of a FP arithmetic expression is forbidden
// The following only works in Clang
// For GCC the compiler flag -ffp-contract=off is used
#pragma STDC FP_CONTRACT OFF

// Malcolm's Algorithm
int radix()
{
    double A, B;

    A = 1.0;
    while ((A + 1.0) - A == 1.0)
    {
        A *= 2.0;
    }

    B = 1.0;
    while ((A + B) - A != B)
    {
        B += 1.0;
    }
    printf("Radix B = %g\n", B);

    return B;
}

// Another Malcolm's Algorithm. This time for computing the precision of a FPS (Floating Point System)
int precision(int B)
{
    int i{0};
    double A{1.0};

    while ((A + 1.0) - A == 1.0)
    {
        A *= B;
        i += 1;
    }
    printf("Precision p = %g\n", i);

    return i;
}

int main()
{
    int B = radix();
    precision(B);

    return 0;
}
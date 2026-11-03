#include <stdio.h>

// Based on Sterbenz's Lemma
// If a and b satifies the following condition, then a - b is exactly representable as a floating point number
bool areSubstractionExactlyRepresentable(double a, double b)
{
    return (b / 2 <= a) && (a <= 2 * b);
}

// Dekker, 1971
// The floating point system being used has radix less than or equal to 3, has subnormal numbers available and provides correct rounding with rounding to nearest.
// If a and b are floating point numbers such that RN(a+b) is a finite fpn, and the exponent of a is larger than or equal to that of be, the Fast2Sum algorithm computes two numbers s and t satisfying:
// s + t = a + b exactly
// s is the closest fpn to a + b
// It is proved that s - a and b - (s - a) are exactly representable, so it cheks out that t is the exact roundoff error, without any information loss.
void fast2Sum(double a, double b, double *s, double *t)
{
    double z;
    *s = a + b;
    z = *s - a;
    *t = b - z;
}

// Knuth and Moller
// Requires 6 floating-point operations instead of 3, but on some processors the penalty due to a wrong branch prediction when comparing a and b costs much more than 3 additional floating-point operations.
// Also we dont need to know if |a| >= |b| and radix greater than 3 is also valid.
// Then we obtain a + b = s +t if a and b are normal floating-point numbers and no underflow or overflow occurs.
// It is optimal among all the RN-addition algorithm without branching.
void twoSum(double a, double b, double *s, double *t)
{
    double c, d, e, f;
    *s = a + b;

    c = *s - b;
    d = *s - c;

    e = a - c;
    f = b - d;

    *t = e + f;
}

int main(int argc, char **argv)
{
    double a, b, s, t;

    /* The inputs are read from the command line. */
    a = 0.7554f;
    b = 0.54f;

    printf("a = %1.16g\n", a);
    printf("b = %1.16g\n", b);
    fast2Sum(a, b, &s, &t);
    printf("s = %1.16g\n", s);
    printf("t = %1.16g\n", t);

    double u, v;
    twoSum(a, b, &s, &t);
    printf("s = %1.16g\n", s);
    printf("t = %1.16g\n", t);
    return 0;
}

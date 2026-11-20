#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <limits>
#include <numbers>
#include <stdexcept>

#include "distributions.h"

// TEMPORARY | FOR NOW ONLY AS A BLACK BOX WITHOUT IMPORTANCE OF THE ABYSMAL STATISTICAL PROPERTIES OF THE FUNCTION BELOW
// Returns a random number in (0, 1)
double generate()
{
    return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
}

// Obtained by the inverse transform algorithm
// Naive way to simulate an exponential random variable with a given rate
// rate > 0
double exponencialRV(double rate)
{
    if (rate <= 0)
    {
        std::cout << "Negative rate value isn't defined" << std::endl;
        return 0;
    }
    return -1.0 / rate * std::log(generate());
}

// Naive way to simulate a gamma random variable with a given n and rate
// When we take Ga(n, r) with n integer, it can be decomposed into the sum of n exponentials with rate r. (This particular case is called the Erlang distribution)
// And we take the log of the product instead of the sum of n logs due to it being way faster computationally, even though it is more prone to numerical instability.
// n, rate > 0, n integer
double erlangRVFirstVersion(unsigned int n, double rate)
{
    if (rate <= 0)
    {
        std::cout << "Negative rate value isn't defined" << std::endl;
        return 0;
    }

    double sum = 0.0;
    double U = 1.0;

    for (unsigned int i = 0; i < n; ++i)
    {
        U *= generate();
        if (U < 1e-200)
        { // Only triggers once every ~460 iterations
            sum -= std::log(U);
            U = 1.0;
        }
    }
    sum -= std::log(U);

    return sum / rate;
}

// We use the rejection technique against an exponential with parameter rate/alpha, which defines the most efficient exponential when generating a gamma variable using the rejection technique.
// This technique allows us to generate a variable X with known density f from a variable Y with density g that we can efficiently simulate. We generate Y and then a random number U. If U <= f(Y) / (c * g(Y)) then X=Y. Otherwise, return to Step 1. Here c is such that f(y) / g(y) <= c, for all y. It is interesting to have c as tight as possible because the number of times that we have to run the algorithm follows a Geometric distribution with mean = c.
// Investigate the Marsaglia–Tsang approach, which is way faster.
// TODO: Implement Two-Sided Kolmogorov–Smirnov (KS) Goodness-of-Fit Test and Pébay's Single-Pass Central Moments to check how good the sampler is.
double gammaRV(double alpha, double scale)
{
    if (!(alpha > 0.0) || !(scale > 0.0) || !std::isfinite(alpha) || !std::isfinite(scale))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // For alpha == 1, Gamma(1, scale) is Exponential(rate = 1 / scale).
    if (alpha == 1.0)
    {
        return exponencialRV(1.0 / scale);
    }

    // Boost trick for 0 < alpha < 1: Gamma(alpha, scale) == Gamma(alpha + 1, scale) * U^(1/alpha)
    if (alpha < 1.0)
    {
        double U = generate();
        while (U <= 0.0)
            U = generate();
        return gammaRV(alpha + 1.0, scale) * std::pow(U, 1.0 / alpha);
    }

    const double a = 1.0 / (alpha * scale);
    const double c = std::pow(alpha, alpha) * std::exp(1.0 - alpha) / std::tgamma(alpha);

    double Y, U;
    do
    {
        Y = exponencialRV(a);
        U = generate();
    } while (U > gamma_pdf(Y, alpha, scale) / (c * exponential_pdf(Y, a)));

    return Y;
}

// To avoid running into overflow problems, we can explicitly simplify the decision boundary at the given point to obtain a simpler expression that drops the need for calling the pdf functions of both the gamma and the exponential as well as taking the comparison into log space. It is also faster.
double gammaRV_safe(double alpha, double scale)
{
    if (!(alpha > 0.0) || !(scale > 0.0) || !std::isfinite(alpha) || !std::isfinite(scale))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (alpha == 1.0)
    {
        return scale * exponencialRV(1.0);
    }

    if (alpha < 1.0)
    {
        double U = generate();
        while (U <= 0.0)
            U = generate();
        return gammaRV_safe(alpha + 1.0, scale) * std::pow(U, 1.0 / alpha);
    }

    while (true)
    {
        const double Z = exponencialRV(1.0); // Z ~ Exp(1), so Y = (alpha * scale) * Z
        const double U = generate();

        if (std::log(U) <= (alpha - 1) * (std::log(Z) + 1.0 - Z))
        {
            return (alpha * scale) * Z;
        }
    }
}

// Simulate a normal variable using standard rejection technique. It has an expected number of iterations of just 1.32 times.
// mean is a real number and standard deviation > 0
double normalRVFirstVersion(double mu, double std)
{
    if (!(std > 0.0) || !std::isfinite(mu) || !std::isfinite(std))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double Y1, Y2, Y;

    while (true)
    {
        Y1 = exponencialRV(1);
        Y2 = exponencialRV(1);
        Y = Y2 - (Y1 - 1) * (Y1 - 1) / 2;

        if (Y > 0)
            break;
    }

    double U{generate()};
    double Z = U <= 1.0 / 2 ? Y1 : -Y1;

    return std * Z + mu;
}

// Simulate a normal variable using Box-Muller transformations.
// It is quite slow due to having to use trigonometric functions.
// mean is a real number and standard deviation > 0
std::array<double, 2> normalRVSecondVersion(double mu, double std)
{
    // Improve
    if (!(std > 0.0) || !std::isfinite(mu) || !std::isfinite(std))
    {
        throw std::invalid_argument("mu must be finite and std > 0");
    }

    double R{std::sqrt(exponencialRV(1. / 2))};
    double angle{generate() * 2 * std::numbers::pi};

    std::array<double, 2> normals{mu + std * R * std::cos(angle), mu + std * R * std::sin(angle)};

    return normals;
}

// Faster from a computational standpoint, even though we on average need to iterate the algorithm more times (2.546 random numbers needed on average).
// Marsaglia polar method
std::array<double, 2> normalRVSecondVersionImproved(double mu, double std)
{
    // Improve
    if (!(std > 0.0) || !std::isfinite(mu) || !std::isfinite(std))
    {
        throw;
    }

    double U1, U2;
    double V1, V2, S;

    while (true)
    {
        U1 = generate();
        U2 = generate();

        V1 = 2 * U1 - 1;
        V2 = 2 * U2 - 1;
        S = V1 * V1 + V2 * V2;

        if (S <= 1)
            break;
    }

    double a = std::sqrt(-2 * std::log(S) / S);
    std::array<double, 2> normals{mu + std * a * V1, mu + std * a * V2};

    return normals;
}

int main()
{
    std::cout << "Simulation of an exponential random variable with rate 4: ";
    std::cout << exponencialRV(4) << " ";
    std::cout << erlangRVFirstVersion(10, 4) << " ";
    std::cout << std::endl;

    float sum{0};
    int N{20000};
    float minimumValue{1};
    float a{0};
    for (int i{0}; i < N; i++)
    {
        a = normalRVSecondVersion(10, 1.0 / 4)[1];
        // a = exponencialRV(4);
        sum += a;
        minimumValue = std::min(minimumValue, a);
    }

    std::cout << "The average is: " << (double)(sum / N) << std::endl;
    std::cout << "The minimum value is: " << a << std::endl;
}
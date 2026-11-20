#pragma once

#include <cmath>

inline double gamma_pdf(double x, double alpha, double beta)
{
    if (x <= 0.0 || alpha <= 0.0 || beta <= 0.0)
    {
        return 0.0;
    }

    double log_pdf = (alpha - 1.0) * std::log(x) - (x / beta) - alpha * std::log(beta) - std::lgamma(alpha);
    return std::exp(log_pdf);
}

// Exponential PDF (rate > 0)
inline double exponential_pdf(double x, double rate)
{
    if (x < 0.0 || rate <= 0.0)
    {
        return 0.0;
    }

    return rate * std::exp(-rate * x);
}
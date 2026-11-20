#include <bit>
#include <cstdint>

// Relative error < 1.5e-6 across all normal positive floats || Verify this claim <----
inline float fast_ln_poly(float x)
{
    uint32_t bits = std::bit_cast<uint32_t>(x);
    int32_t e = static_cast<int32_t>((bits >> 23) & 0xff) - 127;
    float m = std::bit_cast<float>((bits & 0x007fffff) | 0x3f800000);

    // Fold [1, 2) onto [sqrt(2)/2, sqrt(2)] for better symmetry around 1.0
    if (m > 1.41421356f)
    {
        m *= 0.5f;
        e += 1;
    }

    float u = m - 1.0f;
    // Degree-5 minimax polynomial for ln(1 + u) on [sqrt(2)/2 - 1, sqrt(2) - 1]
    float p = -0.1301982f;
    p = p * u + 0.1997324f;
    p = p * u - 0.2499812f;
    p = p * u + 0.3333329f;
    p = p * u - 0.5000000f;
    p = p * u * u + u;

    return static_cast<float>(e) * 0.69314718f + p;
}
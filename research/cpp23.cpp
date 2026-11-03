#include <iostream>
#include <math.h>
#include <stdfloat>

int main()
{
    std::float128_t a = 128.0;
    std::cout << static_cast<float>(a) << std::endl;

    std::float32_t A, B, C;
    A = 10000.0f;
    B = 9999.5f;
    C = 1.0f / 10;
    // It should be 0.6000000015 not 0.599609. It is because A + C is not exactly representable as a floating point number
    std::cout << (A + C) - B << std::endl;

    return 0;
}
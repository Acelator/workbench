#include <iostream>
#include <math.h>
#include <stdfloat>

int main() {
    std::float128_t a = 128.0;
    std::cout << static_cast<float>(a) << std::endl;

    return 0;
}
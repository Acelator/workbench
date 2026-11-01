#include <math.h>
#include <iostream>
#include <limits>

int main()
{
    float a = std::pow(2, -1022);
    double b = std::pow(2, -1022);

    std::cout << std::sin(a) << std::endl;
    std::cout << std::sin(b) << std::endl;

    double p_zero = +0.0;
    double n_zero = -0.0;

    if (p_zero == n_zero)
    {
        // This block WILL execute!
        std::cout << "They are equal under standard comparison." << std::endl;
        std::cout << 1.0 * n_zero << std::endl;
    }

    std::cout << "Minimum normal positive number " << std::numeric_limits<float>::min() << std::endl;
    std::cout << "Minimum subnormal positive number " << std::numeric_limits<float>::denorm_min() << std::endl;
    std::cout << "Bigger finite most negative number " << std::numeric_limits<float>::lowest() << std::endl;
    std::cout << "Distance between 1.0 and the next representable value " << std::numeric_limits<float>::epsilon() << std::endl;

    return 0;
}
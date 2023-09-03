#include "Math.h"

int Math::nearestPowerOfTwo(int n) {
    return std::pow(2, std::floor(std::log2(n)));
}
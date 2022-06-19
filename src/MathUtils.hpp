#ifndef __MATHUTILS_HPP__
#define __MATHUTILS_HPP__

#include <cmath>
#include <limits>
#include <vector>


constexpr float Pi = 3.14;
constexpr float NaN = std::numeric_limits<double>::signaling_NaN();
constexpr float Infinity = std::numeric_limits<double>::infinity();

inline int modulo(int const a, int const b) {
    int m = a % b;
    return m >= 0 ? m : m + b;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline bool between(float x, float a, float b) {
    if (a <= b) return a <= x && x <= b;
    else return b <= x && x <= a;
}

inline float fmedian(std::vector<float> & array) {
    int size = array.size();

    if (size == 0) return 0.;

    for (int i = 0; i < size; i++) {
        float x = array[i];
        int j = i;
        while (j > 0 && x < array[j-1]) {
            array[j] = array[j-1];
            j-=1;
        }
        array[j] = x;
    }
    
    if (size%2 == 0) return (array[(size/2) - 1] + array[size/2]) / 2;
    else return array[size/2];
}


#endif
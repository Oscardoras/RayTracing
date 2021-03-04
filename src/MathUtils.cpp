#ifndef MATHUTILS_C_
#define MATHUTILS_C_

#include <cstdlib>

constexpr float pi = 3.14;
constexpr float NaN = std::numeric_limits<double>::signaling_NaN();
constexpr float Infinite = std::numeric_limits<double>::infinity();

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline double random_int(double min, double max) {
    return int(random_double(min, max+1));
}

inline bool between(float x, float a, float b) {
    if (a <= b) return a <= x && x <= b;
    else return b <= x && x <= a;
}

float med(std::vector<float> array) {
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
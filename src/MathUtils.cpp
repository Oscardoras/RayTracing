#ifndef MATHUTILS_C_
#define MATHUTILS_C_

#include <cstdlib>

constexpr float pi = 3.14;
constexpr float NaN = std::numeric_limits<double>::signaling_NaN();

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    return int(random_double(min, max));
}

float sqrt(float x, float epsilon) {
    float a = 0;
    float b = x;
    float c = (a+b)/2;
    while (b-a > epsilon) {
        float c = (a+b)/2;
        float v = x - c*c;
        if (v >= 0) a = c;
        else b = c;
    }
    return (a+b)/2;
}

float med(std::vector<float> v) {
    int s = v.size();
    if (s > 0) {
        std::sort(v.begin(), v.end());
        if (s%2 == 0) return (v[(s/2) - 1] + v[s/2]) / 2;
        else return v[s/2];
    } else return 0.;
}


#endif
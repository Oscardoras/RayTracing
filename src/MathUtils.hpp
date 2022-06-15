#ifndef __MATHUTILS_HPP__
#define __MATHUTILS_HPP__

#include <cmath>
#include <limits>
#include <vector>


constexpr float Pi = 3.14;
constexpr float NaN = std::numeric_limits<double>::signaling_NaN();
constexpr float Infinity = std::numeric_limits<double>::infinity();

double random_double();

double random_double(double min, double max);

bool between(float x, float a, float b);

float fmedian(std::vector<float> & array);


#endif
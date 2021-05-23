#ifndef OPTICSUTILS_C_
#define OPTICSUTILS_C_

#include <cstdlib>
#include <tuple>

#include "algebra/Vector.hpp"

inline float fresnel(float const& n1, float const& n2, Vector const& normal, Vector const& in) {
    float cos_theta = fmin(-in.unit()*normal, 1.);
    float R0_sqrt = (n1-n2)/(n1+n2);
    float R0 = R0_sqrt*R0_sqrt;
    return R0 + (1.-R0)*pow(1. - cos_theta, 5);
}

inline std::tuple<float, float> fresnelDescartes(float const& n1, float const& n2, Vector const& normal, Vector const& in) {
    float cos_theta = fmin(-in.unit()*normal, 1.);
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    float R0_sqrt = (n1-n2)/(n1+n2);
    float R0 = R0_sqrt*R0_sqrt;
    float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);
    return std::make_tuple(R, sin_theta);
}

inline Vector spRef(Vector const& normal, Vector const& in) {
    return in - 2*(in*normal)*normal;
}

inline Vector spTr(float const& n1, float const& n2, Vector const& normal, Vector const& in) {
    Vector R1 = in.unit();
    Vector R_ort = (n1/n2)*( R1 -(R1*normal)*normal );
    Vector R_para = -sqrt( 1 - R_ort.lengthSquared() )*normal;
    return R_ort + R_para;
}


#endif
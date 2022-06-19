#ifndef __OPTICSUTILS_HPP__
#define __OPTICSUTILS_HPP__

#include <tuple>

#include "algebra/Vector.hpp"

#include "MathUtils.hpp"


/*
inline float getReflectance(float const& n1, float const& n2) {
    float R0_sqrt = (n1-n2)/(n1+n2);
    return R0_sqrt*R0_sqrt;
}
*/

inline Spectrum fresnel(Spectrum const& R0, Vector const& normal, Vector const& in) {
    float const one_minus_cos_theta = 1. - std::abs(in*normal);
    return R0 + (Spectrum::white()-R0) * (one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta);
}

inline std::tuple<float, float> fresnelDescartes(float const n1, float const n2, Vector const& normal, Vector const& in) {
    float const cos_theta = std::abs(in*normal);
    float const one_minus_cos_theta = 1.f - cos_theta;
    float const sin_theta = sqrt(1.f - cos_theta*cos_theta);
    float const R0_sqrt = (n1-n2)/(n1+n2);
    float const R0 = R0_sqrt*R0_sqrt;
    float const R = R0 + (1.f-R0)* one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta*one_minus_cos_theta;
    return std::make_tuple(R, sin_theta);
}

inline Vector specularReflection(Vector const& normal, Vector const& in) {
    return in - 2*(in*normal)*normal;
}

inline Vector specularTransimission(float const n1, float const n2, Vector const& normal, Vector const& in) {
    Vector R1 = in;
    Vector R_ort = (n1/n2)*( R1 -(R1*normal)*normal );
    Vector R_para = -sqrt( 1 - R_ort.lengthSquared() )*normal;
    return R_ort + R_para;
}

inline Light specular(World const& world, Ray const& in, Vector const& out, float const roughness, bool const inside, int const samples, int const depth) {
    int n = std::min(int(roughness*samples), samples);
    if (n <= 1) {
        return world.trace(Ray(in.p, out), inside, samples, depth);
    } else {
        Spectrum spectrum;

        int remaining = std::max(1, samples / n);
        for (int i = 0; i < n; i++)
            spectrum += world.trace(Ray(in.p, out + roughness*Vector::randomUnit()), inside, remaining, depth).compute();

        return Light(0, std::min(int(roughness*10), 3), spectrum / n, Spectrum::white());
    }
}


#endif
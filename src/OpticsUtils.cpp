#ifndef OPTICSUTILS_C_
#define OPTICSUTILS_C_

#include <cstdlib>
#include <tuple>

#include "algebra/Vector.hpp"
#include "algebra/Spectrum.hpp"


inline float getReflectance(float const& n1, float const& n2) {
    float R0_sqrt = (n1-n2)/(n1+n2);
    return R0_sqrt*R0_sqrt;
}

inline Spectrum fresnel(Spectrum const& R0, Vector const& normal, Vector const& in) {
    float cos_theta = fmin(abs(in.unit()*normal), 1.);
    Spectrum f = R0 + (Spectrum(1,1,1)-R0)*pow(1. - cos_theta, 5);
    return f;
}

inline std::tuple<float, float> fresnelDescartes(float const& n1, float const& n2, Vector const& normal, Vector const& in) {
    float cos_theta = fmin(-in.unit()*normal, 1.);
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    float R0_sqrt = (n1-n2)/(n1+n2);
    float R0 = R0_sqrt*R0_sqrt;
    float R = R0 + (1-R0)*pow(1. - cos_theta, 5);
    return std::make_tuple(R, sin_theta);
}

inline Vector specularRef(Vector const& normal, Vector const& in) {
    return in - 2*(in*normal)*normal;
}

inline Vector specularTr(float const& n1, float const& n2, Vector const& normal, Vector const& in) {
    Vector R1 = in.unit();
    Vector R_ort = (n1/n2)*( R1 -(R1*normal)*normal );
    Vector R_para = -sqrt( 1 - R_ort.lengthSquared() )*normal;
    return R_ort + R_para;
}

inline Light specular(bool const& inside, float const& roughness, Vector const& direction, Ray const& in, World const& world, int const& samples, int const& maxDepth) {
    int n = std::max(1, std::min(int(roughness*10 * samples), samples));
    if (n == 1) {
        return world.trace(Ray(in.p, direction), inside, samples, maxDepth);
    } else {
        Spectrum spectrum = Spectrum();
        int remaining = std::max(1, int(samples / n));
        for (int i = 0; i < n; i++) {
            Vector d = (direction + roughness*Vector::randomUnit()).unit();
            spectrum += world.trace(Ray(in.p, d), inside, remaining, maxDepth).compute();
        }
        return Light(0, std::min(int(roughness*10), 3), Spectrum(1,1,1), spectrum)/n;
    }
}


#endif
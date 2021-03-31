#ifndef MATERIALS_SPECULARDIELECTRIC_H_
#define MATERIALS_SPECULARDIELECTRIC_H_

#include <algorithm>
#include <tuple>

#include "Material.hpp"
#include "../World.hpp"


class SpecularDielectric: public Material {

public:

	float ior;
	std::shared_ptr<Texture> fuzziness;

	SpecularDielectric(float ior, std::shared_ptr<Texture> fuzziness) : Material(), ior(ior), fuzziness(fuzziness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		bool out = in.v*faceDirection < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		Vector const& n = (out ? 1 : -1)*faceDirection;//.unit();
		std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in);
		float fresnel = std::get<0>(fd);
		float sin_theta = std::get<1>(fd);
		float f = fuzziness->get(relative.u, relative.v).toBlackAndWhite();

		Light light = Light();
		if ((n1/n2)*sin_theta <= 1.) {
			/*Vector const& R1 = in.v.unit();
			Vector const& R_ort = (n1/n2)*( R1 -(R1*n)*n );
			Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*n;
			Vector const& R2 = R_ort + R_para;
			Light transmitted = world.trace(Ray(in.p, R2), false, samples, maxDepth);
			light += transmitted.addId(long(this) + 2) * (1-R);*/

			light += specularTransmission(n1, n2, f, n, in, world, samples/2, maxDepth).addId(long(this) + 2) * (1-fresnel);
		}
		if (n1 < n2) {
			/*Vector reflect = (in.v - 2*(in.v*faceDirection)*faceDirection).unit();
			Light reflected = world.trace(Ray(in.p, reflect), true, samples, maxDepth);
			light += reflected.addId(long(this) + 4) * R;*/

			light += SpecularMetal::specularReflection(Spectrum(1,1,1), f, faceDirection, in, world, samples/2, maxDepth).addId(long(this) + 4) * fresnel;
		}
		return light;
	}

	static std::tuple<float, float> fresnelDescartes(float const& n1, float const& n2, Vector const& normal, Ray const& in) {
		float cos_theta = fmin(-in.v.unit()*normal, 1.);
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		float R0_sqrt = (n1-n2)/(n1+n2);
		float R0 = R0_sqrt*R0_sqrt;
		float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);
		return std::make_tuple(R, sin_theta);
	}

	static Light specularTransmission(float const& n1, float const& n2, float const& fuzziness, Vector const& normal, Ray const& in, World const& world, int const& samples, int const& maxDepth) {
		Light light = Light();

		Vector const& R1 = in.v.unit();
		Vector const& R_ort = (n1/n2)*( R1 -(R1*normal)*normal );
		Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*normal;
		Vector const& R2 = R_ort + R_para;

		int n = std::max(1, std::min(int(fuzziness*10 * samples), samples));
		int remaining = std::max(1, int(samples / n));
		for (int i = 0; i < n; i++) {
			Vector d = (R2 + fuzziness*Vector::randomUnit()).unit();
			light += world.trace(Ray(in.p, d), false, remaining, maxDepth);
		}

		return light/n;
	}

};


#endif

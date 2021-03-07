#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include <algorithm>
#include <iostream>

#include "Material.hpp"
#include "../World.hpp"


class Dielectric: public Material {

public:

	float ior;

	Dielectric(float ior) : Material(), ior(ior) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		bool out = ray.direction*faceDirection < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		
		Vector const& n = (out ? 1 : -1)*faceDirection.unit();
		float cos_theta = fmin(-ray.direction.unit()*n, 1.);
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		float R0_sqrt = (n1-n2)/(n1+n2);
		float R0 = R0_sqrt*R0_sqrt;
		float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);

		Light light = Light();
		if ((n1/n2)*sin_theta <= 1.) {
			Vector const& R1 = ray.direction.unit();
			Vector const& R_ort = (n1/n2)*( R1 -(R1*n)*n );
			Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*n;
			Vector const& R2 = R_ort + R_para;
			Light transmitted = world.trace(Ray(ray.origin, R2), false, samples, maxDepth);
			transmitted.addId(long(this) + 2);
			light += transmitted * (1-R);
		}
		if (n1 < n2) {
			Vector reflect = (ray.direction - 2*(ray.direction*faceDirection)*faceDirection).unit();
			Light reflected = world.trace(Ray(ray.origin, reflect), true, samples, maxDepth);
			reflected.addId(long(this) + 4);
			light += reflected * R;
		}
		return light;
	}

};


#endif

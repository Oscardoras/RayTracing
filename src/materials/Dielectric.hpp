#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include <algorithm>
#include <iostream>

#include "Material.hpp"
#include "../World.hpp"


class Dielectric: public Material {

public:

	Color albedo;
	float ior;

	Dielectric(Color albedo, float ior) : Material(), albedo(albedo), ior(ior) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const override {
		bool out = ray.direction*faceDirection < 0.;
		float n1 = ray.ior;
		float n2 = out ? ior : 1;
		
		float cos_theta = fmin(-1*ray.direction.unit()*faceDirection.unit(), 1.);
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		float R0_sqrt = (n1-n2)/(n1+n2);
		float R0 = R0_sqrt*R0_sqrt;
		float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);

		Light transmitted = Light(long(this));
		if ((n1/n2)*sin_theta <= 1.) {
			Vector const& n = faceDirection;
			Vector const& R1 = ray.direction;
			Vector const& R_ort = (n1/n2)*( R1 -(R1*n)*n );
			Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*n;
			Vector const& R2 = R_ort + R_para;

			transmitted += world.trace(Ray(ray.origin, R2), !out, std::max(1, int(remainingRays/2)), maxDepth);
		}
		Light specular = Light(long(this));
		{
			Vector reflect = (ray.direction - 2*(ray.direction*faceDirection)*faceDirection).unit();
			specular += world.trace(Ray(ray.origin, reflect), true, std::max(1, int(remainingRays/2)), maxDepth);
		}

		return Light(Spectrum(), transmitted, specular, Spectrum(), Spectrum(), (transmitted.id + specular.id)/2);
	}

};


#endif

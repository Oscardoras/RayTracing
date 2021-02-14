#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include <algorithm>
#include <iostream>

#include "Material.hpp"
#include "../World.hpp"


class Dielectric: public Material {

public:

	Color albedo;
	float refractive_index;

	Dielectric(Color albedo, float refractive_index) : Material(), albedo(albedo), refractive_index(refractive_index) {}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, Hittable const& hittable, float const& u, float const& v, int const& remaningRays, int const& maxDepth) const override {
		Color albe;
		Color light;
		long id = long(this);

		bool out = reflected*faceDirection > 0.;
		float n1;
		float n2;
		if (!out) {
			n1 = 1.;
			n2 = refractive_index;
		} else {
			n1 = refractive_index;
			n2 = 1.;
		}
		
		float cos_theta = fmin(-1*reflected.unit()*faceDirection.unit(), 1.);
        float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		float R0_sqrt = (n1-n2)/(n1+n2);
		float R0 = R0_sqrt*R0_sqrt;
		float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);

		if ((n1/n2)*sin_theta <= 1.) {
			Vector const& n = faceDirection;
			Vector const& R1 = reflected;
			Vector const& R_ort = (n1/n2)*( R1 -(R1*n)*n );
			Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*n;
			Vector const& R2 = R_ort + R_para;

			LightData lightData = world.trace(Ray(point, R2), std::max(1, int(remaningRays/2)), maxDepth-1);
			albe += (1-R)*lightData.albedo;
			light += (1-R)*lightData.light;
			id = (id + lightData.id)/2;
		}
		{
			Vector reflect = (reflected - 2*(reflected*faceDirection)*faceDirection).unit();
			LightData lightData = world.trace(Ray(point, reflect), std::max(1, int(remaningRays/2)), maxDepth-1);

			albe += R*lightData.albedo;
			light += R*lightData.light;
			id = (id + lightData.id)/2;
		}

		return LightData(albedo*albe, light, id);
	}

};


#endif

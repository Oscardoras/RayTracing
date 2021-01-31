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

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const override {
		Color albe;
		Color light;

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
		if ((n1/n2)*sin_theta <= 1.) {
			Vector const& n = faceDirection;
			Vector const& R1 = reflected;
			Vector const& R_ort = (n1/n2)*( R1 -(R1*n)*n );
			Vector const& R_para = -sqrt( 1 - R_ort.lengthSquared() )*n;
			Vector const& R2 = R_ort + R_para;
			/*
			Vector const& t = cross(n, cross(n, V2));
			float sin_theta_1 = (n2/n1) * sqrt(1 - pow( n*V2 / ( n.length() * V2.length() ) , 2) );
			float cos_theta_1 = sqrt(1 - sin_theta_1*sin_theta_1);
			Vector refract = cos_theta_1*(n.unit()) + sin_theta_1*(t.unit());*/

			LightData lightData = world.trace(Ray(point, R2), std::max(1, int(remaningRays/2)), maxDepth-1);
			albe += lightData.albedo;
			light += lightData.light;
		} else {
			Vector reflect = (reflected - 2*(reflected*faceDirection)*faceDirection).unit();
			LightData lightData = world.trace(Ray(point, reflect), std::max(1, int(remaningRays/2)), maxDepth-1);

			albe += lightData.albedo;
			light += lightData.light;
		}

		return LightData(albedo*albe, light, long(this));
	}

};


#endif

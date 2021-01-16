#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"


class Dielectric: public Material {

public:

	Color albedo;
	float refractive_index;

	Dielectric(Color albedo, float refractive_index) : Material(), albedo(albedo), refractive_index(refractive_index) {}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const override {
		Color color;
		Point p = point + 0.001*faceDirection;

		float n1;
		float n2;
		if (dot(reflected, faceDirection) < 0.) {
			n1 = refractive_index;
			n2 = 1;
		} else {
			n1 = 1;
			n2 = refractive_index;
		}
		Vector const& n = faceDirection;
		Vector const& V2 = reflected;
		Vector const& t = cross(n, cross(n, V2));
		float sin_theta_1 = (n2/n1) * sqrt(1 - pow( dot(n, V2) / ( n.length() * V2.length() ) , 2) );
		float cos_theta_1 = sqrt(1 - sin_theta_1*sin_theta_1);
		Vector refract = cos_theta_1*n + sin_theta_1*t;
		LightData lightData = world.trace(Ray(p, refract), std::max(1, int(remaningRays/2)), maxDepth-1);
		color += lightData.light*lightData.albedo;

		Vector reflect = (reflected - 2*dot(reflected, faceDirection)*faceDirection).unit();
		//color += world.trace(Ray(p, reflect), std::max(1, int(remaningRays/2)), maxDepth-1);

		return LightData(albedo, color/2, long(this));
	}

};


#endif

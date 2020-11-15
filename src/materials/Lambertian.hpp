#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Material.hpp"
#include "../World.hpp"


class Lambertian: public Material {

public:

	Color albedo;

	Lambertian(Color albedo) : Material(), albedo(albedo) {}
	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const {
		Color color;
		Point p = point + 0.001*faceDirection;
		for (int i = 0; i < remaningRays; i++) {
			Vector v = Vector::random().unit();
			if (dot(v, faceDirection) < 0) v *= -1;
			Ray r(p, v);
			Color c = world.trace(r, 1, maxDepth-1);
			color += c;
		}
		return (color/remaningRays)*albedo;
	}

};


#endif

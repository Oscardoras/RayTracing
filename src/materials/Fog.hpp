#ifndef MATERIALS_LIGHT_H_
#define MATERIALS_LIGHT_H_

#include "Material.hpp"
#include "../World.hpp"


class Light: public Material {

public:

	Color albedo;
	Hittable hittable;

	Light(Color albedo, Hittable hittable) : Material(), albedo(albedo), hittable(hittable) {}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, float const& u, float const& v, int const& remaningRays, int const& maxDepth) const override {
		return LightData(albedo, long(this));
	}

};


#endif

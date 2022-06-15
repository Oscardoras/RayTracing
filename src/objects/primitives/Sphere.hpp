#ifndef __OBJECTS_PRIMITIVES_SPHERE_H__
#define __OBJECTS_PRIMITIVES_SPHERE_H__

#include "../../algebra/Orientation.hpp"
#include "../../materials/Material.hpp"


class Sphere: public Primitive {

public:

	Point center;
	float radius;
	Material* material;

	Sphere(Point const& center, float const radius, Material* const material);

	virtual float hit(Ray const& r, float const tMin, float const tMax, bool const inside) const override;
	virtual Light color(World const& world, Ray const& in, int const samples, int const depth) const override;

};


#endif

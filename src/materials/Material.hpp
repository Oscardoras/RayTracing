#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/Light.hpp"
#include "../algebra/Point.hpp"


class RelativePosition {

public:

	Vector position;
	float u;
	float v;

	RelativePosition(Vector const& position, float const& u, float const& v) : position(position), u(u), v(v) {}

};


class Material {

public:

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const = 0;

};


#endif

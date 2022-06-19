#ifndef __OBJECTS_PRIMITIVES_PRIMITIVE_H__
#define __OBJECTS_PRIMITIVES_PRIMITIVE_H__

#include "../../algebra/Ray.hpp"
#include "../../algebra/Light.hpp"

#include "../../images/Image.hpp"

#include "../Object.hpp"

struct World;


struct RelativePosition {

	Vector relative;
	float u;
	float v;
	Vector uDirection;
	Vector vDirection;
	Vector normal;
	
	inline RelativePosition(Vector const& relative, float const u, float const v, Vector const& uDirection, Vector const& vDirection, Vector const& normal):
		relative(relative), u(u), v(v), uDirection(uDirection), vDirection(vDirection), normal(normal) {}

};


struct Primitive: public Object {

	inline virtual std::vector<Primitive*> getPrimitives() override {
		return std::vector<Primitive*> {this};
	}

	virtual float hit(Ray const& r, float const tMin, float const tMax, bool const inside) const = 0;
	virtual Light color(World const& world, Ray const& in, int const samples, int const depth) const = 0;

	virtual Ray getNormalRay(int const u, int const v) const = 0;

};


#endif

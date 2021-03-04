#ifndef OBJECTS_OBJECT_H_
#define OBJECTS_OBJECT_H_

#include "../algebra/Ray.hpp"
#include "../algebra/Light.hpp"

class World;
class Box;


class Object {

public:

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& in) const = 0;

	virtual Light color(Ray const& ray, World const& world, int const& maxDepth) const = 0;

	virtual Box getBox() const = 0;

};


#endif

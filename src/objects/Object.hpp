#ifndef OBJECTS_OBJECT_H_
#define OBJECTS_OBJECT_H_

#include "../algebra/Ray.hpp"
#include "../algebra/Light.hpp"
#include "../textures/ImageTexture.hpp"

class World;
class Box;


class RelativePosition {

public:

	Vector position;
	float u;
	float v;

	RelativePosition(Vector const& position, float const& u, float const& v) : position(position), u(u), v(v) {}

};


class Object {

public:

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& in) const = 0;

	virtual Light color(Ray const& in, World const& world, int const& samples, int const& maxDepth) const = 0;

	virtual std::shared_ptr<ImageTexture> getTextureShape(Image const& image) const = 0;

	virtual Ray getSurface(RelativePosition const& relative) const = 0;

	virtual Box getBox() const = 0;

};


#endif

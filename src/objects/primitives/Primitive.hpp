#ifndef OBJECTS_PRIMITIVES_PRIMITIVE_H_
#define OBJECTS_PRIMITIVES_PRIMITIVE_H_

#include "../../algebra/Ray.hpp"
#include "../../algebra/Light.hpp"
#include "../../algebra/Orientation.hpp"
#include "../../images/Image.hpp"
#include "../Object.hpp"

class World;
class Box;
class ImageTexture;


class RelativePosition {

public:

	Vector position;
	float u;
	float v;

	RelativePosition(Vector const& position, float const& u, float const& v) : position(position), u(u), v(v) {}

};

class FaceDirection {

public:

	Vector u;
	Vector w;

	inline Vector v() const {
		return cross(w, u);
	}

	FaceDirection() : u(), w() {}
	FaceDirection(Vector u, Vector w) : u(u), w(w) {}

};


class Primitive: public Object {

public:

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& inside) const = 0;

	virtual Light color(Ray const& in, World const& world, int const& samples, int const& maxDepth) const = 0;

	virtual std::shared_ptr<ImageTexture> getTextureShape(std::shared_ptr<Image> const& image) const = 0;

	virtual Ray getSurface(RelativePosition const& relative) const = 0;

	virtual Box getBox() const = 0;

};


#endif

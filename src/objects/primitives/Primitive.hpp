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
	
	RelativePosition(Vector const& relative, float const u, float const v, Vector const& uDirection, Vector const& vDirection, Vector const& normal);

};


struct Primitive: public Object {

	virtual std::vector<Primitive*> getPrimitives() override;

	virtual float hit(Ray const& r, float const tMin, float const tMax, bool const inside) const = 0;
	virtual Light color(World const& world, Ray const& in, int const samples, int const depth) const = 0;

/*
	virtual ImageTexture* getTextureShape(Image* const& image) const = 0;
	virtual Ray getSurface(int const u, int const v) const = 0;
	virtual Box getBox() const = 0;
*/

};


#endif

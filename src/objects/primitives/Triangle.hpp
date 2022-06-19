#ifndef __OBJECTS_PRIMITIVES_TRIANGLE_H__
#define __OBJECTS_PRIMITIVES_TRIANGLE_H__

#include "../../materials/Material.hpp"


struct Transformation {
	
	virtual Vector transform(Vector const& vector) const = 0;

};


class Triangle: public Primitive {

protected:

	Point A;
	Point B;
	Point C;
	Vector V;
	float d;
	Vector AB_norm;
	Vector AB_ort;
	Vector BC_ort;
	Vector CA_ort;
	Material* material;
	Transformation* transformation;

public:

	Triangle(Point const& A, Point const& B, Point const& C, Material* const& material, Transformation* transformation = nullptr);

	virtual float hit(Ray const& r, float const tMin, float const tMax, bool const inside) const override;
	virtual Light color(World const& world, Ray const& in, int const samples, int const depth) const override;

	virtual Ray getNormalRay(int const u, int const v) const override;

	virtual Box* getBox() const override;

};


#endif

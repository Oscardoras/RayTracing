#ifndef OBJECTS_RECTANGLE_H_
#define OBJECTS_RECTANGLE_H_

#include <memory>
#include <algorithm>

#include "Object.hpp"
#include "primitives/Triangle.hpp"


class RectangleTransformation: public Transformation {

public:

	Vector b;

	RectangleTransformation(Vector b, std::shared_ptr<Transformation> transformation = nullptr):
		Transformation(transformation), b(b) {}

	virtual Vector transform(Vector const& vector) const {
		return b + Transformation::transform(vector);
	}

};


class Rectangle: public Object {

public:

	std::shared_ptr<Triangle> T1;
	std::shared_ptr<Triangle> T2;

	Rectangle(Point A, Point B, Point C, std::shared_ptr<Material> material, std::shared_ptr<Transformation> transformation = std::make_shared<Transformation>()) {
		Point D = A + (C-B);
		T1 = std::make_shared<Triangle>(A, B, D, material, transformation);
		T2 = std::make_shared<Triangle>(C, D, B, material, std::make_shared<RectangleTransformation>((B-A) + (C-B), transformation));
	}

	virtual std::vector<std::shared_ptr<Primitive>> getPrimitives() const override {
		std::vector<std::shared_ptr<Primitive>> primitives = { T1, T2 };
		return primitives;
	}

};


#endif

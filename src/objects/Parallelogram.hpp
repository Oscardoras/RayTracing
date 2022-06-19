#ifndef __OBJECTS_PARALLELOGRAM_H__
#define __OBJECTS_PARALLELOGRAM_H__

#include "Object.hpp"
#include "primitives/Triangle.hpp"


struct ParallelogramTransformation: public Transformation {

	Vector b;

	ParallelogramTransformation(Vector b);

	virtual Vector transform(Vector const& vector) const;

};


class Parallelogram: public Object {

protected:

	ParallelogramTransformation tr;
	Triangle T1;
	Triangle T2;

public:

	Parallelogram(Point const& A, Point const& B, Point const& C, Material* const material);

	virtual std::vector<Primitive*> getPrimitives() override;

};


#endif

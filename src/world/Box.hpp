#ifndef __WORLD_BOX_H__
#define __WORLD_BOX_H__

#include <vector>

#include "../objects/primitives/Primitive.hpp"

#include "../MathUtils.hpp"


class Box {

public:

	Point M;
	Point m;
	std::vector<Primitive const*> primitives;
	std::vector<Box const*> boxes;

	inline Box() {}
	inline Box(Point const& M, Point const& m): M(M), m(m) {}

	~Box();

	Hit hit(Ray const& ray, float const tMin, float const tMax, bool const inside) const;

	void print(int const space = 0) const;

protected:

	bool hit(Ray const& ray, float const tMax) const;

};


bool xComparator(Box* const a, Box* const b);

bool yComparator(Box* const a, Box* const b);

bool zComparator(Box* const a, Box* const b);


#endif

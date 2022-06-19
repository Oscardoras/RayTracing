#ifndef __WORLDS_SIMPLE_WORLD_H__
#define __WORLDS_SIMPLE_WORLD_H__

#include <memory>
#include <vector>

#include "../objects/primitives/Primitive.hpp"

#include "World.hpp"


class SimpleWorld: public World {

public:

	std::vector<Primitive const *> primitives;
	std::unique_ptr<long> nbr;

	inline SimpleWorld(): nbr(std::unique_ptr<long>(new long)) {}

	virtual Light infiniteLight(Ray const& r) const = 0;
	virtual Light maxDepthLight(Ray const& r) const = 0;

	virtual Hit hit(Ray const& ray, bool const inside) const override;
	virtual Light trace(Ray const& ray, bool const inside, int const samples, int const depth) const override;

};


#endif

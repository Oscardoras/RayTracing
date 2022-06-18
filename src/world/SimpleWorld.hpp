#ifndef __WORLDS_SIMPLE_WORLD_H__
#define __WORLDS_SIMPLE_WORLD_H__

#include <memory>
#include <vector>

#include "../objects/primitives/Primitive.hpp"

#include "World.hpp"


class SimpleWorld: public World {

public:

	std::vector<Object*> objects;
	std::unique_ptr<long> nbr;

	SimpleWorld();

	virtual Spectrum infiniteSpectrum(Ray const& r) const = 0;
	virtual Spectrum maxDepthSpectrum(Ray const& r) const = 0;

	virtual Hit hit(Ray const& ray, bool const inside) const;
	virtual Light trace(Ray const& ray, bool const inside, int const samples, int const depth) const override;

};


#endif

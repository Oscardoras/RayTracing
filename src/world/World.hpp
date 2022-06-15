#ifndef __WORLDS_WORLD_H__
#define __WORLDS_WORLD_H__

#include "../algebra/Light.hpp"
#include "../algebra/Ray.hpp"


struct World {

	virtual Light trace(Ray const& ray, bool const inside, int const samples, int const depth) const = 0;

};


#endif

#ifndef WORLD_H_
#define WORLD_H_

#include <limits>
#include <memory>
#include <vector>

#include "algebra/Ray.h"
#include "hittables/Hittable.h"


class World {

private:

	std::vector<std::shared_ptr<Hittable>> hittables;

public:

	virtual ~World();
	virtual Color infiniteColor(Ray const& r) const = 0;

	void isEmpty() {
		hittables.empty();
	}

	void add(std::shared_ptr<Hittable> hittable) {
		hittables.push_back(hittable);
	}

	void clear() {
		hittables.clear();
	}

	Color trace(Ray const& r, int remaningRays) {
		for (const std::shared_ptr<Hittable> hittable : hittables) {
			Hit hit = hittable->hit(r, 0, std::numeric_limits<double>::infinity(), remaningRays);
			if (hit.hitten) return hit;
		}

		return infiniteColor(r);
	}

};

World::~World() {}


#endif

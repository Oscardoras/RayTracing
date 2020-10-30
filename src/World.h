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

	Color trace(Ray const& r, int remaningRays, int maxDepth) {
		for (const std::shared_ptr<Hittable> hittable : hittables) {
			Hit hit = hittable->hit(this, r, 0, std::numeric_limits<double>::infinity(), remaningRays, maxDepth);
			if (hit.hitten) return hit;
		}

		return infiniteColor(r);
	}

};


#endif

#ifndef WORLD_H_
#define WORLD_H_

#include <limits>
#include <memory>
#include <vector>
#include <cmath>

#include "algebra/LightData.hpp"
#include "algebra/Ray.hpp"
#include "hittables/Hittable.hpp"


class Priority {

public:

	Point center;
	float radius;

	Priority(Point center, float radius) : center(center), radius(radius) {}

	bool hit(Ray const& r) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = dot(oc, r.direction);
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
			return halfB*halfB > a*c;
		}
		return false;
	}

};

class World {

public:

	std::vector<std::shared_ptr<Hittable>> hittables;
	std::vector<std::shared_ptr<Priority>> priorities;
	float priority = 0.5;

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

	LightData trace(Ray const& r, int const& remaningRays, int const& maxDepth) const {
		n++;
		if (maxDepth > 0) {
			float tMain = std::numeric_limits<double>::infinity();
			std::shared_ptr<Hittable> hittableMain;
			for (const std::shared_ptr<Hittable> hittable : hittables) {
				float t = hittable->hit(r, tMain);
				if (!std::isnan(t) && t < tMain) {
					tMain = t;
					hittableMain = hittable;
				}
			}

			if (std::isfinite(tMain)) return hittableMain->color(*this, r.at(tMain), r.direction, remaningRays, maxDepth);
			else return LightData(infiniteColor(r), 0);

		} else return LightData();
	}

};


#endif

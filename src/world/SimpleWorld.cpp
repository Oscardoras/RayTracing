#include "SimpleWorld.hpp"

#include "../MathUtils.hpp"
	
	
Hit SimpleWorld::hit(Ray const& ray, bool inside) const {
	if (nbr != nullptr) (*nbr)++;
	Hit hit;
	hit.t = Infinity;
	for (Object* object : objects) {
		for (Primitive* primitive : object->getPrimitives()) {
			float t = primitive->hit(ray, 0.001, hit.t, inside);
			if (!std::isnan(t) && t < hit.t) {
				hit.t = t;
				hit.primitive = primitive;
			}
		}
	}
	if (!std::isfinite(hit.t)) hit.t = NaN;
	return hit;
}

Light SimpleWorld::trace(Ray const& ray, bool inside, int samples, int depth) const {
	if (depth > 0) {
		Hit hit = this->hit(ray, inside);
		if (!std::isnan(hit.t)) {
			Ray ray = Ray(ray.at(hit.t), ray.v);
			return hit.primitive->color(*this, ray, samples, depth-1);
		} else return Light(infiniteSpectrum(ray));
	} else return Light(maxDepthSpectrum(ray));
}
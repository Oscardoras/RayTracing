#include "SimpleWorld.hpp"

#include "../MathUtils.hpp"


SimpleWorld::SimpleWorld(): nbr(std::unique_ptr<long>(new long)) {}
	
Hit SimpleWorld::hit(Ray const& ray, bool const inside) const {
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

Light SimpleWorld::trace(Ray const& ray, bool const inside, int const samples, int const depth) const {
	if (depth > 0) {
		Hit hit = this->hit(ray, inside);
		if (!std::isnan(hit.t)) {
			return hit.primitive->color(*this, Ray(ray.at(hit.t), ray.v.unit()), samples, depth-1);
		} else return Light(infiniteSpectrum(ray));
	} else return Light(maxDepthSpectrum(ray));
}
#include "SimpleWorld.hpp"

#include "../MathUtils.hpp"


Hit SimpleWorld::hit(Ray const& ray, bool const inside) const {
	(*nbr)++;
	Hit hit;
	hit.t = Infinity;
	for (Primitive const * const primitive : primitives) {
		float const t = primitive->hit(ray, 0.001f, hit.t, inside);
		if (!std::isnan(t) && t < hit.t) {
			hit.t = t;
			hit.primitive = primitive;
		}
	}
	if (!std::isfinite(hit.t)) hit.t = NaN;
	return hit;
}

Light SimpleWorld::trace(Ray const& ray, bool const inside, int const samples, int const depth) const {
	if (depth > 0) {
		Hit hit = this->hit(ray, inside);
		if (!std::isnan(hit.t))
			return hit.primitive->color(*this, Ray(ray.at(hit.t), ray.v.unit()), samples, depth-1);
		else
			return infiniteLight(ray);
	} else return maxDepthLight(ray);
}
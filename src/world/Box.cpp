#include <iostream>

#include "Box.hpp"


Box::~Box() {
	for (Box const * box : boxes)
		delete box;
}

Hit Box::hit(Ray const& ray, float const tMin, float const tMax, bool const inside) const {
	Hit hit;
	hit.t = Infinity;
	if (this->hit(ray, tMax)) {
		for (Primitive const * const primitive : primitives) {
			float const t = primitive->hit(ray, tMin, hit.t, inside);
			if (!std::isnan(t) && t < hit.t) {
				hit.t = t;
				hit.primitive = primitive;
			}
		}
		for (Box const * const box : boxes) {
			Hit h = box->hit(ray, tMin, hit.t, inside);
			if (!std::isnan(h.t) && h.t < hit.t)
				hit = h;
		}
	}
	if (!std::isfinite(hit.t)) hit.t = NaN;
	return hit;
}

void Box::print(int const n) const {
	std::string space(n, '\t');

	std::cout << space << "{" << std::endl;
	std::cout << space << "M:" << M.x << "," << M.y << "," << M.z << std::endl;
	std::cout << space << "m:" << m.x << "," << m.y << "," << m.z << std::endl;
	std::cout << space << "primitives:[" << std::endl;
	for (Primitive const * const primitive : primitives)
		std::cout << space << "primitive" << std::endl;
	std::cout << space << "]" << std::endl;
	std::cout << space << "boxes:[" << std::endl;
	for (Box const * const box : boxes)
		box->print(n+1);
	std::cout << space << "]" << std::endl;
	std::cout << space << "}" << std::endl;
}

bool Box::hit(Ray const& ray, float const tMax) const {
	{
		float invD = 1. / ray.v.x;
		float t1 = (m.x - ray.p.x) * invD;
		float t2 = (M.x - ray.p.x) * invD;
		if (invD < 0.) std::swap(t1 , t2);
		if ((t2 < tMax ? t2 : tMax) < (t1 > 0 ? t1 : 0)) return false;
	}
	{
		float invD = 1. / ray.v.y;
		float t1 = (m.y - ray.p.y) * invD;
		float t2 = (M.y - ray.p.y) * invD;
		if (invD < 0.) std::swap(t1 , t2);
		if ((t2 < tMax ? t2 : tMax) < (t1 > 0 ? t1 : 0)) return false;
	}
	{
		float invD = 1. / ray.v.z;
		float t1 = (m.z - ray.p.z) * invD;
		float t2 = (M.z - ray.p.z) * invD;
		if (invD < 0.) std::swap(t1 , t2);
		if ((t2 < tMax ? t2 : tMax) < (t1 > 0 ? t1 : 0)) return false;
	}

	return true;
}


bool xComparator(Box* const a, Box* const b) {
	return a->m.x < b->m.x;
}

bool yComparator(Box* const a, Box* const b) {
	return a->m.y < b->m.y;
}

bool zComparator(Box* const a, Box* const b) {
	return a->m.z < b->m.z;
}

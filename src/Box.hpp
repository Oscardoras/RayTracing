#ifndef BOX_H_
#define BOX_H_

#include <memory>
#include <vector>

#include "algebra/Ray.hpp"
#include "objects/primitives/Primitive.hpp"


class Box {

public:

	Point M;
	Point m;
	std::vector<std::shared_ptr<Primitive>> primitives;
	std::vector<std::shared_ptr<Box>> boxes;

protected:

	bool hit(Ray const& ray, float const& tMax) const {
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

public:

	Box(): m(Point()), M(Point()) {}
	Box(Point M, Point m): M(M), m(m) {}

	Hit hit(Ray const& ray, float const& tMin, float const& tMax, bool const& inside) const {
		Hit hit;
		hit.t = Infinite;
		if (this->hit(ray, tMax)) {
			for (std::shared_ptr<Primitive> const& primitive : primitives) {
				float t = primitive->hit(ray, tMin, hit.t, inside);
				if (!std::isnan(t) && t < hit.t) {
					hit.t = t;
					hit.primitive = primitive;
				}
			}
			for (std::shared_ptr<Box> const& box : boxes) {
				Hit h = box->hit(ray, tMin, hit.t, inside);
				if (!std::isnan(h.t) && h.t < hit.t) {
					hit = h;
				}
			}
		}
		if (!std::isfinite(hit.t)) hit.t = NaN;
		return hit;
	}

	void print(std::string space = "") {
		std::cout << space << "{" << std::endl;
		std::cout << space << "M:" << M.x << "," << M.y << "," << M.z << std::endl;
		std::cout << space << "m:" << m.x << "," << m.y << "," << m.z << std::endl;
		std::cout << space << "primitives:[" << std::endl;
		for (std::shared_ptr<Primitive> primitive : primitives) {
			std::cout << space << "primitive" << std::endl;
		}
		std::cout << space << "]" << std::endl;
		std::cout << space << "boxes:[" << std::endl;
		for (std::shared_ptr<Box> box : boxes) {
			box->print(space + "	");
		}
		std::cout << space << "]" << std::endl;
		std::cout << space << "}" << std::endl;
	}

};


bool xComparator(std::shared_ptr<Box> const& a, std::shared_ptr<Box> const& b) {
	return a->m.x < b->m.x;
}

bool yComparator(std::shared_ptr<Box> const& a, std::shared_ptr<Box> const& b) {
	return a->m.y < b->m.y;
}

bool zComparator(std::shared_ptr<Box> const& a, std::shared_ptr<Box> const& b) {
	return a->m.z < b->m.z;
}


#endif

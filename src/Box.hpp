#ifndef BOX_H_
#define BOX_H_

#include <memory>
#include <vector>

#include "algebra/Ray.hpp"
#include "objects/Object.hpp"


class Box {

public:

	Point M;
	Point m;
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Box>> boxes;

private:

	bool hit(Ray const& ray, float const& tMax) const {
		{
			float invD = 1. / ray.direction.x;
			float t1 = (m.x - ray.origin.x) * invD;
			float t2 = (M.x - ray.origin.x) * invD;
			if (invD < 0.) std::swap(t1 , t2);
			if ((t2 < tMax ? t2 : tMax) <= (t1 > 0 ? t1 : 0)) return false;
		}
		{
			float invD = 1. / ray.direction.y;
			float t1 = (m.y - ray.origin.y) * invD;
			float t2 = (M.y - ray.origin.y) * invD;
			if (invD < 0.) std::swap(t1 , t2);
			if ((t2 < tMax ? t2 : tMax) <= (t1 > 0 ? t1 : 0)) return false;
		}
		{
			float invD = 1. / ray.direction.z;
			float t1 = (m.z - ray.origin.z) * invD;
			float t2 = (M.z - ray.origin.z) * invD;
			if (invD < 0.) std::swap(t1 , t2);
			if ((t2 < tMax ? t2 : tMax) <= (t1 > 0 ? t1 : 0)) return false;
		}
		return true;
	}

public:

	Box(): m(Point()), M(Point()) {}
	Box(Point const& M, Point const& m): M(M), m(m) {}

	Hit hit(Ray const& ray, float const& tMin, float const& tMax, bool const& in) const {
		Hit hit;
		hit.t = Infinite;
		if (this->hit(ray, tMax)) {
			for (const std::shared_ptr<Object> object : objects) {
				float t = object->hit(ray, 0.001, hit.t, in);
				if (!std::isnan(t) && t < hit.t) {
					hit.t = t;
					hit.object = object;
				}
			}
			for (const std::shared_ptr<Box> box : boxes) {
				Hit h = box->hit(ray, 0.001, hit.t, in);
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
		std::cout << space << "objects:[" << std::endl;
		for (std::shared_ptr<Object> object : objects) {
			std::cout << space << "object" << std::endl;
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


bool xComparator(std::shared_ptr<Box> const a, std::shared_ptr<Box> const b) {
	return a->m.x < b->m.x;
}

bool yComparator(std::shared_ptr<Box> const a, std::shared_ptr<Box> const b) {
	return a->m.y < b->m.y;
}

bool zComparator(std::shared_ptr<Box> const a, std::shared_ptr<Box> const b) {
	return a->m.z < b->m.z;
}


#endif

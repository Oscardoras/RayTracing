#ifndef OBJECTS_TRIANGLE_H_
#define OBJECTS_TRIANGLE_H_

#include <memory>
#include <algorithm>

#include "../algebra/Point.hpp"
#include "Object.hpp"
#include "../materials/Material.hpp"


class Triangle: public Object {

public:

	Point A;
	Point B;
	Point C;
	Vector V;
	float d;
	Vector AB_ort;
	Vector BC_ort;
	Vector CA_ort;
	std::shared_ptr<Material> material;

	Triangle(Point A, Point B, Point C, std::shared_ptr<Material> material): Object(), A(A), B(B), C(C), material(material) {
		Vector AB = B-A;
		Vector BC = C-B;
		Vector CA = A-C;
		V = cross(AB,BC);
		d = V*(A.toVector());
		Vector AB_norm = AB.unit();
		Vector BC_norm = BC.unit();
		Vector CA_norm = CA.unit();
		AB_ort = ( BC_norm - (BC_norm*AB_norm)*AB_norm ).unit();
		BC_ort = ( CA_norm - (CA_norm*BC_norm)*BC_norm ).unit();
		CA_ort = ( AB_norm - (AB_norm*CA_norm)*CA_norm ).unit();
	}

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& in) const {
		float l = V * r.direction;
		if (l != 0.) {
			float t = (d - V * (r.origin.toVector())) / l;
			if (tMin < t && t < tMax) {
				Point p = r.at(t);
				if ((p-A)*AB_ort > 0) {
					if ((p-B)*BC_ort > 0) {
						if ((p-C)*CA_ort > 0) {
							return t;
						}
					}
				}
			}
		}

		return NaN;
	}

	virtual Light color(Ray const& ray, World const& world, int const& maxDepth) const override {
		return material->color(RelativePosition(ray.origin - A, (ray.origin-A)*CA_ort, (ray.origin-A)*AB_ort), V*ray.direction < 0 ? V : -1*V, ray, world, maxDepth);
	}

	virtual Box getBox() const {
		Box box = Box(Point(), Point());
		box.M.x = std::max(std::max(A.x, B.x), C.x);
		box.M.y = std::max(std::max(A.y, B.y), C.y);
		box.M.z = std::max(std::max(A.z, B.z), C.z);
		box.m.x = std::min(std::min(A.x, B.x), C.x);
		box.m.y = std::min(std::min(A.y, B.y), C.y);
		box.m.z = std::min(std::min(A.z, B.z), C.z);
		box.objects.push_back(std::make_shared<Triangle>(*this));
		return box;
	}

};


#endif

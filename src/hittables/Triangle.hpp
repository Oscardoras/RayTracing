#ifndef HITTABLES_TRIANGLE_H_
#define HITTABLES_TRIANGLE_H_

#include <memory>
#include <algorithm>

#include "../algebra/Point.hpp"
#include "Hittable.hpp"
#include "../materials/Material.hpp"


class Triangle: public Hittable {

public:

	Point A;
	Point B;
	Point C;
	Vector V;
	float d;
	float AB2;
	float BC2;
	float CA2;
	std::shared_ptr<Material> material;

	Triangle(Point A, Point B, Point C, std::shared_ptr<Material> material): Hittable(), A(A), B(B), C(C), material(material) {
		Vector AB = B-A;
		Vector BC = C-B;
		Vector CA = A-C;
		V = cross(AB,BC);
		d = V*A;
		AB2 = AB.lengthSquared();
		BC2 = BC.lengthSquared();
		CA2 = CA.lengthSquared();
	}

	virtual float hit(Ray const& r, float const& tMax) const {
		float l = V * r.direction;
		if (l != 0.) {
			float t = (d - V * r.origin) / l;
			Point p = r.at(t);
			float AP2 = (A - p).lengthSquared();
			if (AP2 <= AB2 && AP2 <= CA2) {
				float BP2 = (B - p).lengthSquared();
				if (BP2 <= AB2 && BP2 <= BC2) {
					float CP2 = (C - p).lengthSquared();
					if (CP2 <= BC2 && CP2 <= CA2) {
						if (0 < t && t < tMax) return t;
					}
				}
			}
		}

		return NaN;
	}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, int const& remaningRays, int const& maxDepth) const override {
		Vector faceDirection(1., 0., 0.);
		return material->color(world, point, reflected, faceDirection*reflected < 0 ? faceDirection : -1*faceDirection, remaningRays, maxDepth);
	}

};


#endif

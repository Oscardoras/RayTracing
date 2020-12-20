#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"


class Metal: public Material {

public:

	Color albedo;
	float fuzziness;

	Metal(Color albedo, float fuzziness) : Material(), albedo(albedo), fuzziness(fuzziness) {}
	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const {
		Color color;
		Point p = point + 0.001*faceDirection;
		int n = std::max(1, std::min(int(fuzziness * remaningRays), remaningRays));
		int remaining = std::max(1, int(remaningRays / n));
		for (int i = 0; i < n; i++) {
			Vector d = (reflected - 2*dot(reflected, faceDirection)*faceDirection + fuzziness*Vector::random()).unit();
			color += world.trace(Ray(p, d), remaining, maxDepth-1);
		}
		return (color/n)*albedo;
	}

};


#endif

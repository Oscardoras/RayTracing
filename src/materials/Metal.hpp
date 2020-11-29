#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include "Material.hpp"
#include "../World.hpp"


class Metal: public Material {

public:

	Color albedo;
	float fuzziness;

	Metal(Color albedo, float fuzziness) : Material(), albedo(albedo), fuzziness(fuzziness) {}
	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const {
		Point p = point + 0.001*faceDirection;
		Vector d = (reflected - 2*dot(reflected, faceDirection)*faceDirection + fuzziness*Vector::random()).unit();
		Ray r(p, d);
		Color color = world.trace(r, remaningRays, maxDepth-1);;
		return color*albedo;
	}

};


#endif

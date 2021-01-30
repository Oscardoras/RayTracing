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

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const override {
		Color albe;
		Color light;
		long id = 0;

		Point p = point + 0.001*faceDirection;
		int n = std::max(1, std::min(int(fuzziness * remaningRays), remaningRays));
		int remaining = std::max(1, int(remaningRays / n));
		for (int i = 0; i < n; i++) {
			Vector d = (reflected - 2*(reflected*faceDirection)*faceDirection + fuzziness*Vector::random()).unit();
			LightData lightData = world.trace(Ray(p, d), remaining, maxDepth-1);

			albe += lightData.albedo;
			light += lightData.light;
			if (i == 0) id = (lightData.id + long(this))/2;
		}

		return LightData(albedo*albe/n, light/n, id);
	}

};


#endif

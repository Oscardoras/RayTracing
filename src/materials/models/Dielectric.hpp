#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include <algorithm>
#include <tuple>

#include "../../OpticsUtils.cpp"
#include "../Material.hpp"
#include "../../World.hpp"


class Dielectric: public virtual Material {

public:

	float ior;
	std::shared_ptr<Texture> roughness;

	Dielectric(float ior, std::shared_ptr<Texture> roughness) : Material(), ior(ior), roughness(roughness) {}

	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		bool out = in.v*faceDirection.w < 0.;
		Vector const& n = (out ? 1 : -1)*faceDirection.w;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in.v);
		float f = std::get<0>(fd);
		float sin_theta = std::get<1>(fd);
		float r = roughness->getFloat(relative);
		bool tr = (n1/n2)*sin_theta <= 1.;
		bool rf = n1 < n2;
		int s = tr && rf ? std::max(samples/2, 1) : samples;

		Light light = Light();
		if (tr)
			light += (1-f) * specular(!out, r, specularTr(n1, n2, n, in.v), in, world, s, maxDepth).addId(long(this) + 2);
		if (rf)
			light += f * specular(true, r, specularRef(faceDirection.w, in.v), in, world, s, maxDepth).addId(long(this) + 4);
		return light;
	}

};


#endif

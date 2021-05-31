#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include <algorithm>

#include "../../OpticsUtils.cpp"
#include "../Material.hpp"
#include "../../World.hpp"
#include "../../textures/Texture.hpp"


class Metal: public virtual Material {

public:

	std::shared_ptr<Texture> reflection;
	std::shared_ptr<Texture> roughness;

	Metal(std::shared_ptr<Texture> reflection, std::shared_ptr<Texture> roughness) : Material(), reflection(reflection), roughness(roughness) {}

	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum f = fresnel(reflection->getSpectrum(relative), faceDirection.w, in.v);
		float r = roughness->getFloat(relative);
		return f * specular(true, r, specularRef(faceDirection.w, in.v), in, world, samples, maxDepth).addId(long(this));
	}

};


#endif

#ifndef MATERIALS_GLOSSY_H_
#define MATERIALS_GLOSSY_H_

#include "Lambertian.hpp"


class Glossy: public Lambertian {

public:

	std::shared_ptr<Texture> ior;
	std::shared_ptr<Texture> roughness;

	Glossy(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> ior, std::shared_ptr<Texture> roughness, std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
	Lambertian(albedo, priorities, lightMap), ior(ior), roughness(roughness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		float ior = this->ior->getFloat(relative);
		bool out = in.v*faceDirection < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		
		Vector const& n = (out ? 1 : -1)*faceDirection;
		std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in.v);
		float fresnel = std::get<0>(fd);
		float sin_theta = std::get<1>(fd);
		float f = roughness->getFloat(relative);
		int s = std::max(samples/2, 1);

		Light light = Light();
		if ((n1/n2)*sin_theta <= 1.) {
			Spectrum scattered = scatter(relative, faceDirection, in, world, s, maxDepth) * (1-fresnel);
			light += Light(long(this) + 2, 3, albedo->getSpectrum(relative), scattered);
		}
		light += SpecularMetal::specularReflection(f, faceDirection, in, world, s, maxDepth).addId(long(this) + 4) * fresnel;

		return light;
	}

};


#endif

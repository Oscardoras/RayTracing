#ifndef MATERIALS_BDF_H_
#define MATERIALS_BDF_H_

#include "models/Lambertian.hpp"
#include "models/Lamp.hpp"
#include "models/SpecularDielectric.hpp"
#include "models/SpecularMetal.hpp"


class BDF: public Lambertian {

public:

	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Texture> reflection;
	std::shared_ptr<Texture> roughness;
	std::shared_ptr<Texture> ior;
	std::shared_ptr<Texture> normal;

	std::shared_ptr<ImageTexture> lightMap;
	std::shared_ptr<ImageTexture> specularMap;

	BDF(std::shared_ptr<Texture> albedo, std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		Lambertian(albedo, priorities, nullptr) {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Vector normal = faceDirection;
		float ior = this->ior->getFloat(relative);
		bool out = in.v*normal < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		
		Vector const& n = (out ? 1 : -1)*normal;
		std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in.v);
		float fresnel = std::get<0>(fd);
		float sin_theta = std::get<1>(fd);
		float f = roughness->getFloat(relative);
		int s = std::max(samples/2, 1);

		Light light = Light();
		if ((n1/n2)*sin_theta <= 1.) {
			Spectrum scattered = scatter(relative, normal, in, world, s, maxDepth) * (1-fresnel);
			light += Light(long(this) + 2, 3, albedo->getSpectrum(relative), scattered);
		}
		light += reflection->getSpectrum(relative) * SpecularMetal::specularReflection(f, normal, in, world, s, maxDepth).addId(long(this) + 4) * fresnel;

		return light;
	}

};


#endif

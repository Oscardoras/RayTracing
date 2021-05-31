#ifndef MATERIALS_BDF_H_
#define MATERIALS_BDF_H_

#include "models/Lambertian.hpp"
#include "models/Lamp.hpp"
#include "models/Metal.hpp"


class BDF: public Lambertian, public Metal, public Lamp {

public:

	std::shared_ptr<Texture> normal;

	BDF(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> reflection, std::shared_ptr<Texture> emmited, std::shared_ptr<Texture> roughness, std::shared_ptr<Texture> normal, std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		Lambertian(diffuse, priorities, lightMap), Metal(reflection, roughness), Lamp(emmited), normal(normal) {}
	
	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Vector normal;
		if (this->normal == nullptr) normal = faceDirection.w;
		else {
			Vector u = faceDirection.u;
			Vector v = faceDirection.v();
			Vector w = faceDirection.w;
			Vector n = 2*this->normal->getVector(relative) - Vector(1,1,1);
			normal = n.x*u + n.y*v + n.z*w;
		}

		Spectrum diffuse = this->diffuse->getSpectrum(relative);
		Spectrum reflection = this->reflection->getSpectrum(relative);
		float r = roughness->getFloat(relative);
		Spectrum f = fresnel(reflection, normal, in.v);
		int s = std::max(samples/2, 1);
		int s2 = samples-s;

		Light light = Light();
		if (!diffuse.isNull()) light += (Spectrum(1,1,1)-f) * Light(long(this), 3, diffuse, scatter(relative, normal, in, world, s, maxDepth));
		if (!reflection.isNull() && s2 > 0) light += f * specular(true, r, specularRef(normal, in.v), in, world, s2, maxDepth).addId(long(this) + 4);
		light += Light(emitted->getSpectrum(relative)).addId(long(this) + 6);

		return light;
	}

};


#endif

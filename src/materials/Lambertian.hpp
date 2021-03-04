#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Material.hpp"
#include "../textures/Texture.hpp"
#include "../World.hpp"


class Lambertian: public Material {

public:

	std::shared_ptr<Texture> texture;

	Lambertian(std::shared_ptr<Texture> texture) : Material(), texture(texture) {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const override {
		Spectrum sp;
		for (int i = 0; i < remainingRays; i++) {
			Vector vec;
			float p;
			if (!world.priorities.empty() && i < remainingRays*world.priority) {
				std::shared_ptr<Priority> priority = world.priorities[i%world.priorities.size()];
				float radius  = priority->radius;
				vec = (priority->center + Vector::random()*radius) - ray.origin;
				p = ( (pi*radius*radius) / vec.lengthSquared() ) / (4*pi*1*1);
				if (vec*faceDirection < 0) vec *= -1;

				vec = vec.unit();
			} else {
				vec = Vector::randomUnit();
				if (vec*faceDirection < 0) vec *= -1;
				p = 1;
			}
			Light light = world.trace(Ray(ray.origin, vec), true, 1, maxDepth);
			sp += light.compute()*p;
		}

		return Light(Spectrum(), Light(), Light(), texture->get(relative.u, relative.v).toSpectrum(), sp/remainingRays, long(this));
	}

	/*
	- On tire dans chaque région privilégiée et on divise à chaque fois par le nombre de rayons tirés dans la région.
	- On tire les rayons normaux et on divise par le nombre de rayons normaux tirés.
	- On pondère la lumière de chaque région par sa taille.
	- On somme les lumières pondérés.
	- On divise le tout par la somme des pondérations.
	*/

};


#endif

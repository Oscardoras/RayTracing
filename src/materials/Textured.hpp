#ifndef MATERIALS_TEXTURED_H_
#define MATERIALS_TEXTURED_H_

#include "Material.hpp"
#include "../textures/Texture.hpp"
#include "../World.hpp"

#include "../textures/Plain.hpp"
#include "../textures/ImageTexture.hpp"


class Textured: public Material {

//transmited ligt
//emmited light
//diffused light
//specular light

public:

	std::shared_ptr<Texture> albedo = std::make_shared<Plain>(1,1,1);//std::make_shared<ImageTexture>(Image("earthmap.jpg"), pi, pi/2);
	std::shared_ptr<Texture> emitted = std::make_shared<Plain>(0,0,0);
	std::shared_ptr<Texture> ior;
	std::shared_ptr<Texture> fuzziness = std::make_shared<Plain>(1,1,1);

	Textured() : Material() {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		int const& u = relative.u;
		int const& v = relative.v;

		float fuzz = fuzziness->get(u, v).bw();
		int rays = std::max(1, int(samples*fuzz));
		int remaining = int(samples / rays);

		Color reflection;
		for (int i = 0; i < rays; i++) {
			Vector vec;
			do vec = faceDirection + fuzz*Vector::random();
			while (vec*faceDirection < 0);
			Light lightData = world.trace(Ray(ray.origin, vec.unit()), true, remaining, maxDepth);
			reflection += lightData.illumination*lightData.albedo + lightData.emmited;
		}

		return Light(albedo->get(relative.u, relative.v), reflection/rays, light->get(u, v), long(this));
	}

};


#endif

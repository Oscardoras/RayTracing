#ifndef MATERIALS_GLOSSY_H_
#define MATERIALS_GLOSSY_H_

#include <algorithm>
#include <iostream>

#include "Material.hpp"
#include "../World.hpp"


class Glossy: public Material {

public:

	std::shared_ptr<Texture> ior;
	std::shared_ptr<Texture> fuzziness;
	std::shared_ptr<Material> material;

	Glossy(std::shared_ptr<Texture> ior, std::shared_ptr<Texture> fuzziness, std::shared_ptr<Material> material) : Material(), ior(ior), fuzziness(fuzziness), material(material) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		/*bool out = in.v*faceDirection < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		
		Vector const& n = (out ? 1 : -1)*faceDirection.unit();
		float cos_theta = fmin(-in.v.unit()*n, 1.);
		float R0_sqrt = (n1-n2)/(n1+n2);
		float R0 = R0_sqrt*R0_sqrt;
		float R = R0 + (1.-R0)*pow(1. - cos_theta, 5);

		Light light = Light();
		light += material->color(relative, faceDirection, in, world, samples, maxDepth).addId(long(this) + 2) * (1-R);
		if (n1 < n2) {
			Vector reflect = (in.v - 2*(in.v*faceDirection)*faceDirection).unit();
			Light reflected = world.trace(Ray(in.p, reflect), true, samples, maxDepth);
			light += reflected.addId(long(this) + 4) * R;
		}
		return light;*/

		float ior = this->ior->get(relative.u, relative.v).toBlackAndWhite();
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
		
		Vector const& n = (out ? 1 : -1)*faceDirection;//.unit();
		std::tuple<float, float> fd = SpecularDielectric::fresnelDescartes(n1, n2, n, in);
		float fresnel = std::get<0>(fd);
		float f = fuzziness->get(relative.u, relative.v).toBlackAndWhite();

		Light light = Light();
		light += material->color(relative, faceDirection, in, world, samples, maxDepth).addId(long(this) + 2) * (1-fresnel);
		if (n1 < n2)
			light += SpecularMetal::specularReflection(Spectrum(1,1,1), f, faceDirection, in, world, samples/2, maxDepth).addId(long(this) + 4) * fresnel;
		return light;
	}

};


#endif

#ifndef MATERIALS_MODELS_LAMBERTIAN_H_
#define MATERIALS_MODELS_LAMBERTIAN_H_

#include "../Priorisable.hpp"
#include "../../textures/ImageTexture.hpp"


class Area {

public:

	std::shared_ptr<Priority> priority;
	float probability;
	Spectrum spectrum;
	int rays;

	Area(std::shared_ptr<Priority> const& priority, Point const& origin): priority(priority), spectrum(), rays(0) {
		probability = ( (Pi*priority->radius*priority->radius) / (priority->center - origin).lengthSquared() ) / (4*Pi*1*1);
	}
};


class Lambertian: public Priorisable {

public:

	std::shared_ptr<Texture> diffuse;
	std::shared_ptr<ImageTexture> lightMap;

	Lambertian(std::shared_ptr<Texture> diffuse, std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		Priorisable(priorities), diffuse(diffuse), lightMap(lightMap) {}
	
	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum diffuse = this->diffuse->getSpectrum(relative);
		Spectrum scattered = diffuse.isNull() ? Spectrum() : scatter(relative, faceDirection.w, in, world, samples, maxDepth);
		return Light(long(this), 3, diffuse, scattered);
	}

	Spectrum scatter(RelativePosition const& relative, Vector const& normal, Ray const& in, World const& world, int const& samples, int const& maxDepth) const {
		float importance = 1;
		float probability = 1;
		std::vector<Area> areas;

		for (std::shared_ptr<Priority> priority : priorities) {
			Area area(priority, in.p);
			importance -= priority->importance;
			probability -= area.probability; //Suppose qu'il n'y a pas de chevauchement.
			int s = int(priority->importance*samples);
			for (int i = 0; i < s; i++) {
				Vector v = ((priority->center + Vector::randomUnit()*priority->radius) - in.p).unit();
				if (v*normal >= 0) {
					area.spectrum += world.trace(Ray(in.p, v), true, 1, maxDepth).compute();
					area.rays++;
				}
			}
			areas.push_back(area);
		}


		Spectrum spectrum;
		if (lightMap == nullptr) {
			int rays = 0;
			int s = int(importance*float(samples));
			for (int i = 0; i < s; i++) {
				bool hit = false;
				Vector vec = Vector::randomUnit();
				if (vec*normal < 0) vec *= -1;
				Ray r = Ray(in.p, vec);
				Spectrum s = world.trace(r, true, 1, maxDepth).compute();
				for (Area area : areas) {
					if (area.priority->hit(r)) {
						hit = true;
						area.spectrum += s;
						area.rays++;
						break;
					}
				}
				if (!hit) {
					spectrum += s;
					rays++;
				}
			}

			if (rays > 0) spectrum /= rays;
		} else spectrum = lightMap->get(relative.u, relative.v).toSpectrum();
		spectrum *= probability;
		for (Area area : areas) if (area.rays > 0) spectrum += area.probability*area.spectrum/area.rays;

		return spectrum;
	}

	void computeLightMap(std::shared_ptr<Primitive> const& primitive, World const& world, int const& width, int const& height, int const& samples, int const& maxDepth) {
		lightMap = primitive->getTextureShape(std::make_shared<Image>(width, height));
		for (int y = 0; y < height; y++) {
			std::cout << "Computing line : " << y << std::endl;
			for (int x = 0; x < width; x++) {
				Spectrum spectrum;
				Ray surface = primitive->getSurface(RelativePosition(Vector(), x*lightMap->width/width, y*lightMap->height/height));
				for (int i = 0; i < samples; i++) {
					bool hit = false;
					Vector vec = Vector::randomUnit();
					if (vec*surface.v < 0) vec *= -1;
					Ray r = Ray(surface.p, vec);
					for (std::shared_ptr<Priority> priority : priorities) if (priority->hit(r)) {
						hit = true;
						break;
					}
					Spectrum s = Spectrum();
					if (!hit) {
						s = world.trace(r, true, 1, maxDepth).compute();
						spectrum += s;
					}
				}
				lightMap->image->pixels[y][x] = (spectrum / samples).toColor();
			}
		}
	}

};


#endif

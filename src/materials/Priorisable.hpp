#ifndef PRIORITY_H_
#define PRIORITY_H_

#include <memory>

#include "Material.hpp"
#include "../textures/Texture.hpp"


class Priority {

public:

	Point center;
	float radius;
	float importance; //Un nombre qui devrait être proportionelle à la part lumineuse de la priorité.

	Priority(Point center, float radius, float importance): center(center), radius(radius), importance(importance) {}

	bool hit(Ray const& r) const {
		Vector oc = r.p - center;
		float a = r.v.lengthSquared();
		float halfB = oc*r.v;
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
			return halfB*halfB > a*c;
		}
		return false;
	}

	Vector random(Point const& point) const {
		return (center - point) + radius*Vector::randomUnit();
	}

};

class Area {

public:

	std::shared_ptr<Priority> priority;
	float probability; //Représente la surface angulaire de la zone par la surface d'une demi-sphère.
	Spectrum spectrum;
	int rays;

	Area(std::shared_ptr<Priority> const& priority, Point const& origin): priority(priority), spectrum(), rays(0) {
		probability = ( (Pi*priority->radius*priority->radius) / (priority->center - origin).lengthSquared() ) / (4*Pi*1*1);
	}
};


class Priorisable: public Material {

public:

	std::vector<std::shared_ptr<Priority>> priorities;
	std::shared_ptr<Texture> lightMap;

	Priorisable(std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>()):
		Material(), priorities(priorities) {}
	
	virtual Spectrum sample(Vector const& faceDirection, Ray const& in, Ray &out, World const& world, int const& maxDepth) const = 0;

	Spectrum scatter(Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const {
		float remaind = 1;
		float probability = 1;
		std::vector<Area> areas;

		for (std::shared_ptr<Priority> priority : priorities) {
			Area area(priority, in.p);
			remaind -= priority->importance;
			probability -= area.probability;
			int s = int(priority->importance*float(samples));
			for (int i = 0; i < s; i++) {
				Vector v = (priority->center + Vector::random()*priority->radius) - in.p;
				Ray out = Ray(in.p, v);
				area.spectrum += sample(faceDirection, in, out, world, maxDepth);
				area.rays++;
			}
			areas.push_back(area);
		}


		Spectrum spectrum;
		int rays = 0;
		int s = int(remaind*float(samples));
		for (int i = 0; i < s; i++) {
			bool hit = false;
			Vector v = Vector::randomUnit();
			Ray r = Ray(in.p, v);
			for (Area area : areas) {
				if (area.priority->hit(r)) {
					hit = true;
					area.spectrum += sample(faceDirection, in, r, world, maxDepth);
					area.rays++;
				}
			}
			if (!hit) {
				spectrum += sample(faceDirection, in, r, world, maxDepth);
				rays++;
			}
		}

		if (rays > 0) spectrum /= rays;
		spectrum *= probability;
		for (Area area : areas) if (area.rays > 0) spectrum += area.probability*area.spectrum/area.rays;

		return spectrum;
	}

};


#endif

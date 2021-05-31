#ifndef OBJECTS_PRIMITIVES_SPHERE_H_
#define OBJECTS_PRIMITIVES_SPHERE_H_

#include <memory>
#include <algorithm>

#include "Primitive.hpp"
#include "../../materials/Material.hpp"


class Sphere: public Primitive {

public:

	Point center;
	float radius;
	std::shared_ptr<Material> material;
	Orientation orientation;

	Sphere(Point center, float radius, std::shared_ptr<Material> material, Orientation orientation = Orientation()):
		Primitive(), center(center), radius(radius), material(material), orientation(orientation) {}

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& inside) const {
		Vector oc = r.p - center;

		float l2 = oc.lengthSquared();
		if (inside && l2 + 0.01 < radius*radius) return 0;

		float a = r.v.lengthSquared();
		float halfB = oc*r.v;
		if (halfB < 0) {
			float c = l2 - radius*radius;
			float discriminant = halfB*halfB - a*c;

			if (discriminant > 0) {
				float root = sqrt(discriminant);
				float t = (-halfB - root)/a;
				if (t < tMin) t = (-halfB + root)/a;
				if (tMin < t && t < tMax) return t;
			}
		}

		return NaN;
	}

	virtual Light color(Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Vector oc = in.p - center;
		if (oc.lengthSquared() + 0.01 < radius*radius) return material->color(RelativePosition(oc, NaN, NaN), FaceDirection(), in, world, samples, maxDepth);

		float xz_radius = Vector(oc.x, 0, oc.z).length();
		float theta = std::acos(oc.x / xz_radius);
		if (oc.z > 0) theta = -theta + 2*Pi;
		theta += orientation.yaw;
		theta -= 2*Pi*int(theta/(2*Pi));

		float phi = std::asin(oc.y / radius) + Pi/2;

		Vector w = oc.unit();
		return material->color(RelativePosition(oc, theta*radius, phi*radius), FaceDirection(cross(Vector(1,0,0).unit(), w), w), in, world, samples, maxDepth);
	}

	virtual std::shared_ptr<ImageTexture> getTextureShape(std::shared_ptr<Image> const& image) const override {
		return std::make_shared<ImageTexture>(image, 2*Pi*radius, Pi*radius);
	}

	virtual Ray getSurface(RelativePosition const& relative) const override {
		float theta = (relative.u / radius) - orientation.yaw;
		float phi = relative.v / radius;

		float sin = std::sin(phi - Pi/2);
		float y = sin;
		float xz_radius = sqrt(1 - sin*sin);
		float x = std::cos(theta) * xz_radius;
		float z = std::sin(theta) * xz_radius;

		Point p = center + radius*Vector(x, y, z);
		return Ray(p, (p - center).unit());
	}

	virtual Box getBox() const override {
		Box box = Box(center + radius*Vector(1,1,1), center - radius*Vector(1,1,1));
		return box;
	}

};


#endif

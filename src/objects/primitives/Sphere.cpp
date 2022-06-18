#include "Sphere.hpp"

#include "../../MathUtils.hpp"


Sphere::Sphere(Point const& center, float const radius, Material* const material):
	center(center), radius(radius), material(material) {}

float Sphere::hit(Ray const& r, float const tMin, float const tMax, bool const inside) const {
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

Light Sphere::color(World const& world, Ray const& in, int const samples, int const depth) const {
	Vector oc = in.p - center;
	if (oc.lengthSquared() + 0.01 < radius*radius)
		return material->color(world, in, RelativePosition(oc, NaN, NaN, Vector(), Vector(), oc.unit()), samples, depth);

	float xz_radius = Vector(oc.x, 0, oc.z).length();
	float theta = std::acos(oc.x / xz_radius);
	if (oc.z > 0) theta = -theta + 2*Pi;
	theta -= 2*Pi*int(theta/(2*Pi));

	float phi = std::asin(oc.y / radius) + Pi/2;

	Vector w = oc.unit();
	return material->color(world, in, RelativePosition(oc, theta*radius, phi*radius, Vector(1,0,0), Vector(0,1,0), oc.unit()), samples, depth);
}

/*
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
*/
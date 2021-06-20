#ifndef OBJECTS_PRIMITIVES_TRIANGLE_H_
#define OBJECTS_PRIMITIVES_TRIANGLE_H_

#include <memory>
#include <algorithm>

#include "../../algebra/Point.hpp"
#include "Primitive.hpp"
#include "../../materials/Material.hpp"
#include "../../Box.hpp"


class Triangle: public Primitive {

public:

	Point A;
	Point B;
	Point C;
	Vector V;
	float d;
	Vector AB_ort;
	Vector BC_ort;
	Vector CA_ort;
	std::shared_ptr<Transformation> transformation;
	std::shared_ptr<Material> material;

	Triangle(Point A, Point B, Point C, std::shared_ptr<Material> material, std::shared_ptr<Transformation> transformation = std::make_shared<Transformation>()):
		Primitive(), A(A), B(B), C(C), material(material), transformation(transformation) {
		Vector AB = B-A;
		Vector BC = C-B;
		Vector CA = A-C;
		V = cross(AB,BC).unit();
		d = V*(A - Point());
		Vector AB_norm = AB.unit();
		Vector BC_norm = BC.unit();
		Vector CA_norm = CA.unit();
		AB_ort = ( BC_norm - (BC_norm*AB_norm)*AB_norm ).unit();
		BC_ort = ( CA_norm - (CA_norm*BC_norm)*BC_norm ).unit();
		CA_ort = ( AB_norm - (AB_norm*CA_norm)*CA_norm ).unit();
	}

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& inside) const {
		float l = V * r.v;
		if (l != 0.) {
			float t = (d - V * (r.p - Point())) / l;
			if (tMin < t && t < tMax) {
				Point p = r.at(t);
				if ((p-A)*AB_ort > 0) {
					if ((p-B)*BC_ort > 0) {
						if ((p-C)*CA_ort > 0) {
							return t;
						}
					}
				}
			}
		}

		return NaN;
	}

	virtual Light color(Ray const& in, World const& world, int const& remaningRays, int const& maxDepth) const override {
		Vector vec = transformation->transform(in.p - A);
		return material->color(RelativePosition(vec, vec*CA_ort, vec*AB_ort), FaceDirection(CA_ort, V*in.v < 0 ? V : -V), in, world, remaningRays, maxDepth);
	}

	virtual std::shared_ptr<ImageTexture> getTextureShape(std::shared_ptr<Image> const& image) const override {
		return std::make_shared<ImageTexture>(image, abs((B-A)*CA_ort), abs((C-A)*AB_ort));
	}

	virtual Ray getSurface(RelativePosition const& relative) const override {
		return Ray(A + relative.u*CA_ort + relative.v*AB_ort, Vector());
	}

	virtual Box getBox() const override {
		Box box = Box(Point(), Point());
		box.M.x = std::max(std::max(A.x, B.x), C.x);
		box.M.y = std::max(std::max(A.y, B.y), C.y);
		box.M.z = std::max(std::max(A.z, B.z), C.z);
		box.m.x = std::min(std::min(A.x, B.x), C.x);
		box.m.y = std::min(std::min(A.y, B.y), C.y);
		box.m.z = std::min(std::min(A.z, B.z), C.z);
		return box;
	}

};


#endif
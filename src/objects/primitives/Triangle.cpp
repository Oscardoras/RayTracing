#include "Triangle.hpp"

#include "../../world/Box.hpp"


Triangle::Triangle(Point const& A, Point const& B, Point const& C, Material* const& material, Transformation* transformation):
	A(A), B(B), C(C), material(material), transformation(transformation) {
	Vector AB = B-A;
	Vector BC = C-B;
	Vector CA = A-C;
	AB_norm = AB.unit();
	Vector BC_norm = BC.unit();
	Vector CA_norm = CA.unit();
	V = cross(AB_norm,BC_norm);
	d = V*(A - Point());
	AB_ort = ( BC_norm - (BC_norm*AB_norm)*AB_norm ).unit();
	BC_ort = ( CA_norm - (CA_norm*BC_norm)*BC_norm ).unit();
	CA_ort = ( AB_norm - (AB_norm*CA_norm)*CA_norm ).unit();
}

float Triangle::hit(Ray const& r, float const tMin, float const tMax, bool const inside) const {
	float l = V * r.v;
	if (l < 0.) {
		float t = (d - V * (r.p - Point())) / l;
		if (tMin < t && t < tMax) {
			Point p = r.at(t);
			if ((p-A)*AB_ort > 0)
				if ((p-B)*BC_ort > 0)
					if ((p-C)*CA_ort > 0)
						return t;
		}
	}

	return NaN;
}

Light Triangle::color(World const& world, Ray const& in, int const samples, int const depth) const {
	Vector const vec = transformation == nullptr ? in.p - A : transformation->transform(in.p - A);
	return material->color(world, in, RelativePosition(vec, vec*AB_norm, vec*AB_ort, AB_norm, AB_ort, V*in.v <= 0. ? V : -V), samples, depth);
}

Ray Triangle::getNormalRay(int const u, int const v) const {
	return Ray(A + u*CA_ort + v*AB_ort, V);
}

Box* Triangle::getBox() const {
	Box* box = new Box(Point(), Point());

	box->M.x = std::max(std::max(A.x, B.x), C.x);
	box->M.y = std::max(std::max(A.y, B.y), C.y);
	box->M.z = std::max(std::max(A.z, B.z), C.z);
	box->m.x = std::min(std::min(A.x, B.x), C.x);
	box->m.y = std::min(std::min(A.y, B.y), C.y);
	box->m.z = std::min(std::min(A.z, B.z), C.z);

	return box;
}
#include "Parallelogram.hpp"


ParallelogramTransformation::ParallelogramTransformation(Vector b): b(b) {}

Vector ParallelogramTransformation::transform(Vector const& vector) const {
	return b - vector;
}

Parallelogram::Parallelogram(Point const& A, Point const& B, Point const& C, Material* const material):
	tr(C-A), T1(A, B, A + (C-B), material), T2(C, A + (C-B), B, material, &tr) {}

std::vector<Primitive*> Parallelogram::getPrimitives() {
	return { &T1, &T2 };
}
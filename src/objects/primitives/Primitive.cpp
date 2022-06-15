#include "Primitive.hpp"


RelativePosition::RelativePosition(Vector const& relative, float u, float v, Vector const& uDirection, Vector const& vDirection, Vector const& normal):
	relative(relative), u(u), v(v), uDirection(uDirection), vDirection(vDirection), normal(normal) {}


std::vector<Primitive*> Primitive::getPrimitives() {
	return std::vector<Primitive*> {this};
}
#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/Light.hpp"
#include "../algebra/Point.hpp"
#include "../objects/primitives/Primitive.hpp"


class Material {

public:

	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& remaningRays, int const& maxDepth) const = 0;

};


#endif

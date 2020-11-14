#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/Color.hpp"
#include "../algebra/Point.hpp"


class Material {

public:

	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const = 0;

};


#endif

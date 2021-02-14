#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/LightData.hpp"
#include "../algebra/Point.hpp"


class Material {

public:

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, Hittable const& hittable, float const& u, float const& v, int const& remaningRays, int const& maxDepth) const = 0;

};


#endif

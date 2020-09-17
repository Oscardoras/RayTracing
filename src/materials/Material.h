#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"


class Material {

public:

	virtual ~Material();
	virtual Color hit(Point point, Vector reflected, Vector faceDirection, int remaningRays, std::shared_ptr<World> world) const = 0;

};

Material::~Material() {}


#endif

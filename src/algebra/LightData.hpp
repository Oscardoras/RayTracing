#ifndef ALGEBRA_LIGHTDATA_H_
#define ALGEBRA_LIGHTDATA_H_

#include "Color.hpp"
class Material;


class LightData {

public:

	Color albedo;
	Color light;
	Material *material = nullptr;

	LightData() {}
	LightData(Color albedo): albedo(albedo), light(Color(1., 1., 1.)) {}
	LightData(Color albedo, Color light, Material *material): albedo(albedo), light(light), material(material) {}

};


#endif

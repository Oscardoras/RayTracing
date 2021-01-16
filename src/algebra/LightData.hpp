#ifndef ALGEBRA_LIGHTDATA_H_
#define ALGEBRA_LIGHTDATA_H_

#include <cstdlib>

#include "Color.hpp"
class Material;


class LightData {

public:

	Color albedo;
	Color light;
	long id;

	LightData() {}
	LightData(Color albedo, int id): albedo(albedo), light(Color(1., 1., 1.)), id(id) {}
	LightData(Color albedo, Color light, int id): albedo(albedo), light(light), id(id) {}

};


#endif

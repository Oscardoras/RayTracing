#ifndef ORIENTATION_H_
#define ORIENTATION_H_

#include "Vector.hpp"


class Orientation {

public:

	float yaw;
	float roll;
	float pitch;

	Orientation(): yaw(0), roll(0), pitch(0) {}
	Orientation(float yaw, float roll, float pitch): yaw(yaw), roll(roll), pitch(pitch) {}

};


#endif

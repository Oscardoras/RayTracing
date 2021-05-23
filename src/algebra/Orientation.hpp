#ifndef ALGEBRA_ORIENTATION_H_
#define ALGEBRA_ORIENTATION_H_

#include <cmath>

#include "../MathUtils.cpp"


class Orientation {

public:

	float yaw;
	float roll;
	float pitch;

	Orientation(): yaw(0), roll(0), pitch(0) {}
	Orientation(float yaw, float roll, float z): yaw(yaw), roll(roll), pitch(pitch) {}

	inline Orientation& operator+=(Orientation const& o) {
		yaw += o.yaw;
		roll += o.roll;
		pitch += o.pitch;
		return *this;
	}

	inline Orientation& operator-=(Orientation const& o) {
		yaw -= o.yaw;
		roll -= o.roll;
		pitch -= o.pitch;
		return *this;
	}

	inline static Orientation random() {
		return Orientation(random_double(0, 2*Pi), random_double(0, 2*Pi), random_double(0, 2*Pi));
	}

	inline static float canonical(float const& angle) {
		return angle - 2*Pi*int(angle/(2*Pi));
	}

};

inline Orientation operator+(Orientation const& u, Orientation const& v) {
	return Orientation(u.yaw + v.yaw, u.roll + v.roll, u.pitch + v.pitch);
}

inline Orientation operator-(Orientation const& u, Orientation const& v) {
	return Orientation(u.yaw - v.yaw, u.roll - v.roll, u.pitch - v.pitch);
}

inline Orientation operator-(Orientation const& o) {
	return Orientation(-o.yaw, -o.roll, -o.pitch);
}


#endif

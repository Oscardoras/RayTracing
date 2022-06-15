#ifndef __ALGEBRA_ORIENTATION_H__
#define __ALGEBRA_ORIENTATION_H__


struct Orientation {

	float yaw;
	float roll;
	float pitch;

	Orientation();
	Orientation(float const yaw, float const roll, float const z);

	Orientation& operator+=(Orientation const& o);

	Orientation& operator-=(Orientation const& o);

	static Orientation random();

	static float canonical(float const angle);

};

Orientation operator+(Orientation const& u, Orientation const& v);

Orientation operator-(Orientation const& u, Orientation const& v);

Orientation operator-(Orientation const& o);


#endif

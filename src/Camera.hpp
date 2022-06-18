#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "world/World.hpp"
#include "images/Image.hpp"


class Camera {

public:

	World* world;
	Point position;
	Vector direction;
	Vector top;
	int resolution;
	float ratio;
	float gamma;

	Camera(World* const world, int const resolution, float const ratio, float const gamma);

	void setPosition(Point const& position, Vector const& direction, Vector const& top);

	Image render(int const threads, float const sigma, int const samples, int const depth) const;

protected:

	static void threadRender(Light **lights, int width, int height, int from, int to, World * world, Point const position, Point corner, Vector horizontal, Vector vertical, int samples, int depth);
	static void threadGaussian(Light **lights, Image * image, int from, int to, float g, float sigma);

};


#endif

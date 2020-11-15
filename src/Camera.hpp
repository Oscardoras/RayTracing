#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>

#include "World.hpp"


class Camera {

public:

	std::shared_ptr<World> world;
	Point position;
	Vector direction;
	int resolution;
	float ratio;
	float focalLength;
	float gamma;
	int maxRayPerPixel;
	int maxDepth;

	Camera(std::shared_ptr<World> world, Point position, Vector direction, int resolution, float ratio, float focalLength, float gamma, int maxRayPerPixel, int maxDepth):
		world(world), position(position), direction(direction), resolution(resolution), ratio(ratio), focalLength(focalLength), gamma(gamma), maxRayPerPixel(maxRayPerPixel),  maxDepth(maxDepth) {}

	void render(std::ostream &out) const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Vector vertical = Vector(0.0, 1.0, 0.0);
		Vector horizontal = Vector(ratio, 0.0, 0.0);
		Point corner = position - horizontal/2 - vertical/2 - Vector(0, 0, focalLength);

		float g = 1/gamma;

		out << "P3\n" << width << ' ' << height << "\n255\n";

		for (int j = height-1; j >= 0; --j) {
			std::cerr << "\rLines remaining: " << j << ' ' << std::flush;
			for (int i = 0; i < width; ++i) {
				float u = float(i) / (width-1);
				float v = float(j) / (height-1);
				Ray r(position, corner + u*horizontal + v*vertical - position);
				Color color = world->trace(r, maxRayPerPixel, maxDepth);

				color.r = pow(color.r, g);
				color.g = pow(color.g, g);
				color.b = pow(color.b, g);
				float max = 1.;
				if (color.r > max) max = color.r;
				if (color.g > max) max = color.g;
				if (color.b > max) max = color.b;
				if (max > 1.) color /= max;
				writeColor(out, color);
			}
		}
	}

};


#endif
#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <memory>

#include "algebra/Color.h"
#include "algebra/Point.h"
#include "algebra/Ray.h"
#include "algebra/Vector.h"
#include "World.h"


class Camera {

public:

	std::shared_ptr<World> world;
	Point position;
	Vector direction;
	int resolution;
	float ratio;
	float focalLength;
	int maxRayPerPixel;

	Camera(std::shared_ptr<World> world, Point position, Vector direction, int resolution, float ratio, float focalLength, int maxRayPerPixel):
		world(world), position(position), direction(direction), resolution(resolution), ratio(ratio), focalLength(focalLength), maxRayPerPixel(maxRayPerPixel) {}

	void render(std::ostream &out) const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Vector vertical = Vector(0.0, 1.0, 0.0);
		Vector horizontal = Vector(ratio, 0.0, 0.0);
		Point corner = position - horizontal/2 - vertical/2 - Vector(0, 0, focalLength);

		out << "P3\n" << width << ' ' << height << "\n255\n";

		for (int j = height-1; j >= 0; --j) {
			std::cerr << "\rLines remaining: " << j << ' ' << std::flush;
			for (int i = 0; i < width; ++i) {
				float u = float(i) / (width-1);
				float v = float(j) / (height-1);
				Ray r(position, corner + u*horizontal + v*vertical - position);
				writeColor(out, world->trace(r, maxRayPerPixel));
			}
		}
	}

};


#endif

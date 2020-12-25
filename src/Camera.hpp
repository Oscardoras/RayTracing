#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>
#include <vector>

#include <thread>
#include <mutex>

#include "World.hpp"
#include "Image.hpp"


class Camera {

public:

	std::shared_ptr<World> world;
	Point position;
	Vector direction;
	Vector top;
	int resolution;
	float ratio;
	float gamma;
	int maxRayPerPixel;
	int maxDepth;

	Camera(std::shared_ptr<World> world, Point position, Vector direction, Vector top, int resolution, float ratio, float gamma, int maxRayPerPixel, int maxDepth):
		world(world), position(position), direction(direction), top(top), resolution(resolution), ratio(ratio), gamma(gamma), maxRayPerPixel(maxRayPerPixel),  maxDepth(maxDepth) {}

private:

	static void th(std::mutex *lock, Image *image, std::shared_ptr<World> world, int width, int height, int y, Point position, Point corner, Vector horizontal, Vector vertical, float g, int maxRayPerPixel, int maxDepth) {
		for (int x = 0; x < width; ++x) {
			float u = float(x) / (width-1);
			float v = float(y) / (height-1);
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
			
			lock->lock();
			image->set(x, y, color);
			lock->unlock();
		}
	}

public:

	Image render() const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Vector horizontal = cross(direction, top).unit();
		Vector vertical = cross(horizontal, direction).unit();
		horizontal *= ratio;
		Point corner = position - horizontal/2 - vertical/2 + direction;

		float g = 1/gamma;

		Image image(width, height);

		std::vector<std::shared_ptr<std::thread>> threads;
		std::mutex lock;
		for (int y = height-1; y >= 0; --y) {
			threads.push_back(std::make_shared<std::thread>(Camera::th, &lock, &image, world, width, height, y, position, corner, horizontal, vertical, g, maxRayPerPixel, maxDepth));
		}
    	for (int i = 0; i < threads.size(); i++) threads[i]->join();

		return image;
	}

};


#endif

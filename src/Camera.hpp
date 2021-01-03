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
#include "Rendering.hpp"


class Camera {

public:

	std::shared_ptr<World> world;
	Point position;
	Vector direction;
	Vector top;
	int resolution;
	float ratio;

	Camera(std::shared_ptr<World> world, Point position, Vector direction, Vector top, int resolution, float ratio):
		world(world), position(position), direction(direction), top(top), resolution(resolution), ratio(ratio) {}

private:

	static void th(std::mutex *lock, std::shared_ptr<std::vector<int>> lines, Rendering *rendering, std::shared_ptr<World> world, Point position, Point corner, Vector horizontal, Vector vertical, int step, int maxRayPerPixel, int maxDepth) {
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				std::cout << "Remaining lines : " << lines->size() << std::endl;
				y = lines->front();
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < rendering->width; x++) {
				float u = float(x) / (rendering->width-1);
				float v = float(y) / (rendering->height-1);
				Ray r(position, corner + u*horizontal + v*vertical - position);
				LightData lightData = world->trace(r, y%step == x%step ? maxRayPerPixel : 1, maxDepth);
				
				lock->lock();
				rendering->set(x, y, lightData);
				lock->unlock();
			}
		}
	}

public:

	Rendering render(int t, int step, int maxRayPerPixel, int maxDepth) const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Vector horizontal = cross(direction, top).unit();
		Vector vertical = cross(horizontal, direction).unit();
		horizontal *= ratio;
		Point corner = position - horizontal/2 - vertical/2 + direction;

		Rendering rendering(width, height);

		std::shared_ptr<std::vector<int>> lines = std::make_shared<std::vector<int>>();
		for (int i = height-1; i >= 0; i--) lines->push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads;
		std::mutex lock;
		for (; t > 0; t--) {
			threads.push_back(std::make_shared<std::thread>(Camera::th, &lock, lines, &rendering, world, position, corner, horizontal, vertical, step, maxRayPerPixel, maxDepth));
			//Camera::th(&lock, lines, &rendering, world, position, corner, horizontal, vertical, step, maxRayPerPixel, maxDepth);
		}
    	for (int i = 0; i < threads.size(); i++) threads[i]->join();

		return rendering;
	}

};


#endif

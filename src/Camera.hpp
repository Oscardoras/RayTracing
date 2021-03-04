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
#include "images/Rendering.hpp"


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

protected:

	static void th(std::mutex *lock, std::shared_ptr<std::vector<int>> lines, Rendering *rendering, std::shared_ptr<World> world, Point position, Point corner, Vector horizontal, Vector vertical, bool half, int maxRayPerPixel, int maxDepth) {
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Rendering line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < rendering->width; x++) {
				float u = float(x) / (rendering->width-1);
				float v = float(y) / (rendering->height-1);
				Ray r(position, corner + u*horizontal + v*vertical - position);
				Light light = world->trace(r, true, (!half || x%2 == y%2) ? maxRayPerPixel : 1, maxDepth);
				
				lock->lock();
				rendering->set(x, y, light);
				lock->unlock();
			}
		}
	}

	static void complete(Rendering& rendering) {
		Color **completed = new Color *[rendering.height];
		for (int y = 0; y < rendering.height; y++) {
			std::cout << "Completing line : " << y << std::endl;
			completed[y] = new Color[rendering.width];
			for (int x = 0; x < rendering.width; x++) {
				long id = rendering.ids[y][x];
				std::vector<float> vr;
				std::vector<float> vg;
				std::vector<float> vb;

				if (y%2 == x%2) {
					Color c = rendering.scatterIllumination.pixels[y][x];
					vr.push_back(c.r);
					vg.push_back(c.g);
					vb.push_back(c.b);

					if (y > 0 && x > 0 && rendering.ids[y-1][x-1] == id) {
						Color c = rendering.scatterIllumination.pixels[y-1][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y > 0 && x < rendering.width-1 && rendering.ids[y-1][x+1] == id) {
						Color c = rendering.scatterIllumination.pixels[y-1][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < rendering.height-1 && x < rendering.width-1 && rendering.ids[y+1][x+1] == id) {
						Color c = rendering.scatterIllumination.pixels[y+1][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < rendering.height-1 && x > 0 && rendering.ids[y+1][x-1] == id) {
						Color c = rendering.scatterIllumination.pixels[y+1][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					completed[y][x] = Color(med(vr), med(vg), med(vb));
				} else {
					if (y > 0 && rendering.ids[y-1][x] == id) {
						Color c = rendering.scatterIllumination.pixels[y-1][x];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}																																																																																																																																																																						
					if (x < rendering.width-1 && rendering.ids[y][x+1] == id) {
						Color c = rendering.scatterIllumination.pixels[y][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < rendering.height-1 && rendering.ids[y+1][x] == id) {
						Color c = rendering.scatterIllumination.pixels[y+1][x];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (x > 0 && rendering.ids[y][x-1] == id) {
						Color c = rendering.scatterIllumination.pixels[y][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					completed[y][x] = Color(med(vr), med(vg), med(vb));
				}
			}
		}

		for(int j = 0; j < rendering.height; j++) delete rendering.scatterIllumination.pixels[j];
		delete rendering.scatterIllumination.pixels;
		rendering.scatterIllumination.pixels = completed;
	}

public:

	Rendering render(int t, bool half, int maxRayPerPixel, int maxDepth) const {
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
			threads.push_back(std::make_shared<std::thread>(Camera::th, &lock, lines, &rendering, world, position, corner, horizontal, vertical, half, maxRayPerPixel, maxDepth));
			//Camera::th(&lock, lines, &rendering, world, position, corner, horizontal, vertical, half, maxRayPerPixel, maxDepth);
		}
    	for (int i = 0; i < threads.size(); i++) threads[i]->join();

		if (half) complete(rendering);

		return rendering;
	}

};


#endif

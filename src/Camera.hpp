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

	static void th(std::mutex *lock, std::shared_ptr<std::vector<int>> lines, Rendering *rendering, std::shared_ptr<World> world, Point position, Point corner, Vector horizontal, Vector vertical, bool half, int maxRayPerPixel, int maxDepth) {
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				std::cout << "Remaining lines : " << lines->size() << std::endl;
				y = lines->front();
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			//for (int x = (!half || y%2 == x%2 ? 0 : 1); x < rendering->width; x += (half ? 2 : 1)) {
			for (int x = 0; x < rendering->width; x++) {
				if (x%2 == y%2) {
					float u = float(x) / (rendering->width-1);
					float v = float(y) / (rendering->height-1);
					Ray r(position, corner + u*horizontal + v*vertical - position);
					LightData lightData = world->trace(r, maxRayPerPixel, maxDepth);
					
					lock->lock();
					rendering->set(x, y, lightData);
					lock->unlock();
				}
			}
		}
	}

	static void complete(Rendering& rendering) {
		int& width = rendering.width;
		int& height = rendering.height;
		Color **completed = new Color *[height];
		for(int j = 0; j < height; j++) completed[j] = new Color[width];

		for (int y = 0; y < height; y++) {
			Color *completed = new Color[width];
			for (int x = 0; x < width; x++) {
				long id = rendering.ids[y][x];
				std::vector<float> vr;
				std::vector<float> vg;
				std::vector<float> vb;

				if (y%2 == x%2) {
					/*Color c = rendering->light.pixels[y][x];
					vr.push_back(c.r);
					vg.push_back(c.g);
					vb.push_back(c.b);

					if (y > 0 && x > 0 && rendering->ids[y-1][x-1] == id) {
						Color c = rendering->light.pixels[y-1][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y > 0 && x < width-1 && rendering->ids[y-1][x+1] == id) {
						Color c = rendering->light.pixels[y-1][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < height-1 && x < width-1 && rendering->ids[y+1][x+1] == id) {
						Color c = rendering->light.pixels[y+1][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < height-1 && x > 0 && rendering->ids[y+1][x-1] == id) {
						Color c = rendering->light.pixels[y+1][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					completed[y][x] = Color(med(vr), med(vg), med(vb));*/
					completed[x] = rendering.light.pixels[y][x];
				} else if (false) {
					if (y > 0/* && rendering->ids[y-1][x] == id*/) {
						Color c = rendering.light.pixels[y-1][x];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (x < width-1/* && rendering->ids[y][x+1] == id*/) {
						Color c = rendering.light.pixels[y][x+1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (y < height-1/* && rendering->ids[y+1][x] == id*/) {
						Color c = rendering.light.pixels[y+1][x];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					if (x > 0/* && rendering->ids[y][x-1] == id*/) {
						Color c = rendering.light.pixels[y][x-1];
						vr.push_back(c.r);
						vg.push_back(c.g);
						vb.push_back(c.b);
					}
					completed[x] = Color(med(vr), med(vg), med(vb));
				} else if (x > 0) completed[x] = rendering.light.pixels[y][x-1];
			}
			delete rendering.light.pixels[y];
			rendering.light.pixels[y] = completed;
		}
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

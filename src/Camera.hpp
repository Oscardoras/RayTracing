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

	static void threadRender(std::mutex *lock, std::vector<int> *lines, Rendering *rendering, std::shared_ptr<World> world, Point position, Point corner, Vector horizontal, Vector vertical, bool half, int samples, int maxDepth) {
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Rendering line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < rendering->accurate.width; x++) {
				float u = float(x) / (rendering->accurate.width-1);
				float v = float(y) / (rendering->accurate.height-1);
				Ray r(position, corner + u*horizontal + v*vertical - position);
				Light light = world->trace(r, true, (!half || x%2 == y%2) ? samples : 1, maxDepth);
				
				lock->lock();
				rendering->set(x, y, light);
				lock->unlock();
			}
		}
	}

	static void threadComplete(std::mutex *lock, std::vector<int> *lines, Rendering *rendering, std::vector<Scatter> **completed) {
		int const& width = rendering->accurate.width;
		int const& height = rendering->accurate.height;

		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Completing line : " << y << std::endl;
				lines->erase(lines->begin());
				completed[y] = new std::vector<Scatter>[width];
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				std::vector<Scatter> smooth;
				for (Scatter scatter : rendering->smooth[y][x]) {
					long const& id = scatter.id;
					std::vector<Spectrum> v;

					if (y%2 == x%2) {
						v.push_back(scatter.scattered);

						if (y > 0 && x > 0) {
							for (Scatter s : rendering->smooth[y-1][x-1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
						if (y > 0 && x < width-1) {
							for (Scatter s : rendering->smooth[y-1][x+1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
						if (y < height-1 && x < width-1) {
							for (Scatter s : rendering->smooth[y+1][x+1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
						if (y < height-1 && x > 0) {
							for (Scatter s : rendering->smooth[y+1][x-1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
					} else {
						if (y > 0) {
							for (Scatter s : rendering->smooth[y-1][x]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}																																																																																																																																																																						
						if (x < width-1) {
							for (Scatter s : rendering->smooth[y][x+1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
						if (y < height-1) {
							for (Scatter s : rendering->smooth[y+1][x]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
						if (x > 0) {
							for (Scatter s : rendering->smooth[y][x-1]) if (s.id == id) {
								v.push_back(s.scattered);
								break;
							}
						}
					}
					smooth.push_back(Scatter(id, scatter.albedo, med(v)));
				}
				lock->lock();
				completed[y][x] = smooth;
				lock->unlock();
			}
		}
	}

public:

	Rendering render(int const& t, bool const& half, int const& samples, int const& maxDepth) const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Vector horizontal = cross(direction, top).unit();
		Vector vertical = cross(horizontal, direction).unit();
		horizontal *= ratio;
		Point corner = position - horizontal/2 - vertical/2 + direction;

		Rendering rendering(width, height);

		std::vector<int> lines1 = std::vector<int>();
		for (int i = height-1; i >= 0; i--) lines1.push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads;
		std::mutex lock;
		for (int i = 0; i < t; i++) {
			threads.push_back(std::make_shared<std::thread>(Camera::threadRender, &lock, &lines1, &rendering, world, position, corner, horizontal, vertical, half, samples, maxDepth));
			//Camera::threadRender(&lock, &lines1, &rendering, world, position, corner, horizontal, vertical, half, samples, maxDepth);
		}
    	for (int i = 0; i < threads.size(); i++) threads[i]->join();

		if (half) {
			std::vector<Scatter> **completed = new std::vector<Scatter> *[height];

			std::vector<int> lines2 = std::vector<int>();
			for (int i = height-1; i >= 0; i--) lines2.push_back(i);

			std::vector<std::shared_ptr<std::thread>> threads;
			std::mutex lock;

			for (int i = 0; i < t; i++) {
				threads.push_back(std::make_shared<std::thread>(Camera::threadComplete, &lock, &lines2, &rendering, completed));
				//Camera::threadComplete(&lock, &lines2, &rendering, completed);
			}
			for (int i = 0; i < threads.size(); i++) threads[i]->join();

			for(int j = 0; j < height; j++) delete[] rendering.smooth[j];
			delete[] rendering.smooth;
			rendering.smooth = completed;
		}

		return rendering;
	}

};


#endif

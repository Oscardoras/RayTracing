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
#include "images/Image.hpp"


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

	static void threadRender(std::mutex *lock, std::vector<int> *lines, std::vector<Smooth> **smoothing, Image *image, std::shared_ptr<World> world, Point position, Point corner, Vector horizontal, Vector vertical, bool half, int samples, int maxDepth) {
		int const& width = image->width;
		int const& height = image->height;
		
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Rendering line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				float u = float(x) / (width-1);
				float v = float(y) / (height-1);
				Ray r(position, corner + (1-u)*horizontal + v*vertical - position);
				Light light = world->trace(r, true, (!half || x%2 == y%2) ? samples : 1, maxDepth);
				
				lock->lock();
				smoothing[y][x] = light.smoothing;
				lock->unlock();
			}
		}
	}

	static void threadComplete(std::mutex *lock, std::vector<int> *lines, std::vector<Smooth> **smoothing, Image *image, bool half, float g, int r) {
		int const& width = image->width;
		int const& height = image->height;

		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Completing line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				Color color = Color();

				for (Smooth smooth : smoothing[y][x]) {
					long id = smooth.id;
					int radius = int(smooth.radius * r);
					if (half && radius < 1) radius = 1;

					std::vector<std::vector<float>> m;
					for (int j = -radius; j <= radius; j++) {
						std::vector<float> l;
						for (int i = -radius; i <= radius; i++) {
							if (radius == 0) l.push_back(1.);
							else l.push_back(std::exp( -(i*i + j*j) / (2*radius*radius*9/16)));
						}
						m.push_back(l);
					}

					Spectrum spectrum;
					float sum = 0.;

					for (int j = -radius; j <= radius; j++) {
						for (int i = -radius; i <= radius; i++) {
							if ((!half || (y+j)%2 == (x+i)%2) && y+j >= 0 && y+j <= height-1 && x+i >= 0 && x+i <= width-1) {
								for (Smooth s : smoothing[y+j][x+i]) if (s.id == id) {
									float coef = m[radius + j][radius + i];
									sum += coef;
									spectrum += coef*s.light;
									break;
								}
							}
						}
					}
					color += (smooth.albedo * spectrum/sum).toColor();


					/*std::vector<Spectrum> v;

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
					smooth.push_back(Scatter(id, scatter.albedo, med(v)));*/
				}

				color.r = pow(color.r, g);
				color.g = pow(color.g, g);
				color.b = pow(color.b, g);
				lock->lock();
				image->pixels[y][x] = color;
				lock->unlock();
			}
		}
	}

public:

	Image render(int const& t, float const& gamma, int const& radius, bool const& half, int const& samples, int const& maxDepth) const {
		const int height = resolution;
		const int width = static_cast<int>(resolution * ratio);

		Image image = Image(width, height);

		Vector horizontal = cross(direction, top);
		if (horizontal.lengthSquared() == 0.) {
			if (direction.x != 0. && direction.y != 0.) horizontal = cross(direction, Vector(0,1,0));
			else horizontal = cross(direction, Vector(1,0,0));
		}
		horizontal = horizontal.unit();
		Vector vertical = cross(horizontal, direction).unit();
		horizontal *= ratio;
		Point corner = position - horizontal/2 - vertical/2 + direction;
		float g = 1/gamma;

		std::vector<Smooth> **smoothing = new std::vector<Smooth> *[height];
		for(int j = 0; j < height; j++) smoothing[j] = new std::vector<Smooth> [width];


		std::vector<int> lines1 = std::vector<int>();
		for (int i = height-1; i >= 0; i--) lines1.push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads1;
		std::mutex lock1;
		for (int i = 0; i < t; i++) {
			threads1.push_back(std::make_shared<std::thread>(Camera::threadRender, &lock1, &lines1, smoothing, &image, world, position, corner, horizontal, vertical, half, samples, maxDepth));
			//Camera::threadRender(&lock, &lines1, smoothing, $image, world, position, corner, horizontal, vertical, half, samples, maxDepth);
		}
    	for (int i = 0; i < threads1.size(); i++) threads1[i]->join();


		std::vector<int> lines2 = std::vector<int>();
		for (int i = height-1; i >= 0; i--) lines2.push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads2;
		std::mutex lock2;
		for (int i = 0; i < t; i++) {
			threads2.push_back(std::make_shared<std::thread>(Camera::threadComplete, &lock2, &lines2, smoothing, &image, half, g, radius));
			//Camera::threadComplete(&lock, &lines2, smoothing, &image, half, g, radius);
		}
		for (int i = 0; i < threads2.size(); i++) threads2[i]->join();


		for(int j = 0; j < height; j++) delete[] smoothing[j];
		delete[] smoothing;

		return image;
	}

};


#endif

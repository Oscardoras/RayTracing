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
	float gamma;

	Camera(std::shared_ptr<World> world, Point position, Vector direction, Vector top, int resolution, float ratio, float gamma):
		world(world), position(position), direction(direction), top(top), resolution(resolution), ratio(ratio), gamma(gamma) {}

protected:

	struct Matrice {
		int size;
		std::vector<std::vector<float>> m;
	};

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

	static void threadMedian(std::mutex *lock, std::vector<int> *lines, std::vector<Smooth> **smoothing, int width, int height, bool half) {
		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Median line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				for (Smooth &smooth : smoothing[y][x]) {
					long id = smooth.id;
					std::vector<Spectrum> v;

					for (int j = -1; j <= 1; j++) if (y+j >= 0 && y+j <= height-1) {
						for (int i = -1; i <= 1; i++) if (x+i >= 0 && x+i <= width-1) {
							if (!half || (y+j)%2 == (x+i)%2) {
								for (Smooth s : smoothing[y+j][x+i]) if (s.id == id) {
									v.push_back(s.light);
									break;
								}
							}
						}
					}

					Spectrum sp = med(v);
					lock->lock();
					smooth.light = sp;
					lock->unlock();
				}
			}
		}
	}

	static void threadGaussian(std::mutex *lock, std::vector<int> *lines, std::vector<Smooth> **smoothing, Image *image, bool half, float g, int radius, float sigma) {
		int const& width = image->width;
		int const& height = image->height;

		std::vector<Matrice> matrices;

		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Gaussian line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				Color color = Color();

				for (Smooth &smooth : smoothing[y][x]) {
					long id = smooth.id;
					int r = smooth.radius * radius;
					if (half && r < 1) r = 1;

					Matrice *matrice = nullptr;
					for (Matrice &m : matrices) if (m.size == r) {
						matrice = &m;
						break;
					}
					if (matrice == nullptr) {
						Matrice m;
						m.size = r;
						float o = r * sigma;
						for (int j = -r; j <= r; j++) {
							std::vector<float> l;
							for (int i = -r; i <= r; i++) {
								if (r == 0) l.push_back(1.);
								else l.push_back(std::exp( -(i*i + j*j) / (2*o*o)));
							}
							m.m.push_back(l);
						}
						matrices.push_back(m);
						matrice = &matrices.back();
					}

					Spectrum spectrum;
					float sum = 0.;

					for (int j = -r; j <= r; j++) if (y+j >= 0 && y+j <= height-1) {
						for (int i = -r; i <= r; i++) if (x+i >= 0 && x+i <= width-1) {
							for (Smooth &s : smoothing[y+j][x+i]) if (s.id == id) {
								float coef = matrice->m[r+j][r+i];
								sum += coef;
								spectrum += coef*s.light;
								break;
							}
						}
					}
					color += (smooth.albedo * spectrum/sum).toColor();
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

	Image render(int const& t, bool const& median, int const& radius, float const& sigma, bool const& half, int const& samples, int const& maxDepth) const {
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


		if (median || half) {
			std::vector<int> lines2 = std::vector<int>();
			for (int i = height-1; i >= 0; i--) lines2.push_back(i);

			std::vector<std::shared_ptr<std::thread>> threads2;
			std::mutex lock2;
			for (int i = 0; i < t; i++) {
				threads2.push_back(std::make_shared<std::thread>(Camera::threadMedian, &lock2, &lines2, smoothing, width, height, half));
				//Camera::threadMedian(&lock2, &lines2, smoothing, width, height, half);
			}
			for (int i = 0; i < threads2.size(); i++) threads2[i]->join();
		}


		std::vector<int> lines3 = std::vector<int>();
		for (int i = height-1; i >= 0; i--) lines3.push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads3;
		std::mutex lock3;
		for (int i = 0; i < t; i++) {
			threads3.push_back(std::make_shared<std::thread>(Camera::threadGaussian, &lock3, &lines3, smoothing, &image, half, g, radius, sigma));
			//Camera::threadGaussian(&lock3, &lines3, smoothing, &image, half, g, radius, sigma);
		}
		for (int i = 0; i < threads3.size(); i++) threads3[i]->join();


		for(int j = 0; j < height; j++) delete[] smoothing[j];
		delete[] smoothing;

		return image;
	}

};


#endif

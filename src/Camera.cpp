#include <iostream>
#include <cmath>
#include <vector>
#include <thread>

#include "Camera.hpp"


Camera::Camera(World* const world, int const resolution, float const ratio, float const gamma):
	world(world), resolution(resolution), ratio(ratio), gamma(gamma) {}

void Camera::setPosition(Point const& position, Vector const& direction, Vector const& top) {
	this->position = position;
	this->direction = direction;
	this->top = top;
}

Image Camera::render(int const threads, float const sigma, int const samples, int const depth) const {
	const int height = resolution;
	const int width = resolution * ratio;
	Vector horizontal = cross(direction, top).unit() * ratio;
	Vector vertical = cross(horizontal, direction).unit();
	Point corner = position - horizontal/2 - vertical/2 + direction;
	float g = 1/gamma;

	Image image(width, height);

	std::vector<Smooth> **smoothing = new std::vector<Smooth> *[height];
	for(int j = 0; j < height; j++)
		smoothing[j] = new std::vector<Smooth>[width];

	
	std::vector<std::thread> threads1;
	int h = (height / threads)+1;
	for (int i = 0; i < threads; i++) {
		threads1.push_back(std::thread(Camera::threadRender, smoothing, width, height, i*h, std::min((i+1)*h, height), world, position, corner, horizontal, vertical, samples, depth));
		//Camera::threadRender(smoothing, width, height, i*h, std::min(i*(h+1), height), world, position, corner, horizontal, vertical, samples, depth);
	}
	for (std::thread & thread : threads1)
		thread.join();


	std::vector<std::thread> threads2;
	for (int i = 0; i < threads; i++) {
		threads2.push_back(std::thread(Camera::threadGaussian, smoothing, &image, i*h, std::min((i+1)*h, height), g, sigma));
		//Camera::threadGaussian(smoothing, &image, i*h, std::min(i*(h+1), height), g, sigma);
	}
	for (std::thread & thread : threads2)
		thread.join();


	for(int j = 0; j < height; j++)
		delete[] smoothing[j];
	delete[] smoothing;

	return image;
}

void Camera::threadRender(std::vector<Smooth> **smoothing, int width, int height, int from, int to, World * world, Point const position, Point corner, Vector horizontal, Vector vertical, int samples, int depth) {
	for (int y = from; y < to; y++) {
		std::cout << "Rendering line : " << y << std::endl;

		for (int x = 0; x < width; x++) {
			float u = float(x) / (width-1);
			float v = float(y) / (height-1);
			Ray r(position, corner + (1-u)*horizontal + v*vertical - position);
			Light light = world->trace(r, true, samples, depth);
			
			smoothing[y][x] = light.smoothings;
		}
	}
}

void Camera::threadGaussian(std::vector<Smooth> **smoothing, Image * image, int from, int to, float g, float sigma) {
	std::vector<Matrix> matrices;

	for (int y = from; y < to; y++) {
		std::cout << "Gaussian line : " << y << std::endl;

		for (int x = 0; x < image->width; x++) {
			Color color;

			for (Smooth & smooth : smoothing[y][x]) {
				if (smooth.id != 0 & smooth.radius > 0) {
					int r = (smooth.radius-1)/2;

					Matrix* matrix = nullptr;
					for (Matrix & m : matrices) if (m.size == smooth.radius) {
						matrix = &m;
						break;
					}
					if (matrix == nullptr) {
						matrices.push_back(Matrix(smooth.radius));
						float o = smooth.radius * sigma;
						for (int j = -r; j <= r; j++)
							for (int i = -r; i <= r; i++)
								matrices.back().elements[j+r][i+r] = std::exp( -(i*i + j*j) / (2*o*o));
						matrix = &matrices.back();
					}

					Spectrum spectrum;
					float sum = 0.;
					for (int j = -r; j <= r; j++) if (y+j >= 0 && y+j <= image->height-1) {
						for (int i = -r; i <= r; i++) if (x+i >= 0 && x+i <= image->width-1) {
							for (Smooth &s : smoothing[y+j][x+i]) if (s.id == smooth.id) {
								float coef = matrix->elements[r+j][r+i];
								sum += coef;
								spectrum += coef*s.smoothing;
								break;
							}
						}
					}
					color += (smooth.accurate * spectrum/sum).toColor();
				} else color = (smooth.accurate * smooth.accurate).toColor();
			}

			color.r = pow(color.r, g);
			color.g = pow(color.g, g);
			color.b = pow(color.b, g);

			image->pixels[y][x] = color;
		}
	}
}
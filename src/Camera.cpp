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

	Light **lights = new Light *[height];
	for(int j = 0; j < height; j++)
		lights[j] = new Light[width];

	
	std::vector<std::thread> threads1;
	int h = (height / threads)+1;
	for (int i = 0; i < threads; i++) {
		threads1.push_back(std::thread(Camera::threadRender, lights, width, height, i*h, std::min((i+1)*h, height), world, position, corner, horizontal, vertical, samples, depth));
		//Camera::threadRender(lights, width, height, i*h, std::min(i*(h+1), height), world, position, corner, horizontal, vertical, samples, depth);
	}
	for (std::thread & thread : threads1)
		thread.join();


	std::vector<std::thread> threads2;
	for (int i = 0; i < threads; i++) {
		threads2.push_back(std::thread(Camera::threadGaussian, lights, &image, i*h, std::min((i+1)*h, height), g, sigma));
		//Camera::threadGaussian(lights, &image, i*h, std::min(i*(h+1), height), g, sigma);
	}
	for (std::thread & thread : threads2)
		thread.join();


	for(int j = 0; j < height; j++)
		delete[] lights[j];
	delete[] lights;

	return image;
}

void Camera::threadRender(Light **lights, int width, int height, int from, int to, World * world, Point const position, Point corner, Vector horizontal, Vector vertical, int samples, int depth) {
	for (int y = from; y < to; y++) {
		std::cout << "Rendering line : " << y << std::endl;

		for (int x = 0; x < width; x++) {
			float u = float(x) / (width-1);
			float v = float(y) / (height-1);
			Ray r(position, corner + (1-u)*horizontal + v*vertical - position);
			lights[y][x] = world->trace(r, true, samples, depth);
		}
	}
}

void Camera::threadGaussian(Light **lights, Image * image, int from, int to, float g, float sigma) {
	std::vector<Matrix> matrices;

	for (int y = from; y < to; y++) {
		std::cout << "Gaussian line : " << y << std::endl;

		for (int x = 0; x < image->width; x++) {
			Color color;

			Light & light = lights[y][x];
			if (light.id != 0 && light.radius > 0) {
				int const radius = light.radius;
				int const size = 1 + 2*light.radius;

				Matrix* matrix = nullptr;
				for (Matrix & m : matrices) if (m.size == size) {
					matrix = &m;
					break;
				}
				if (matrix == nullptr) {
					matrices.push_back(Matrix(size));
					float o = radius * sigma;
					for (int j = -radius; j <= radius; j++)
						for (int i = -radius; i <= radius; i++)
							matrices.back().elements[j+radius][i+radius] = std::exp( -(i*i + j*j) / (2*o*o));
					matrix = &matrices.back();
				}

				Spectrum spectrum;
				float sum = 0.;
				for (int j = -radius; j <= radius; j++) if (y+j >= 0 && y+j <= image->height-1) {
					for (int i = -radius; i <= radius; i++) if (x+i >= 0 && x+i <= image->width-1) {
						auto const& light2 = lights[y+j][x+i];
						if (light2.id == light.id) {
							float coef = matrix->elements[radius+j][radius+i];
							sum += coef;
							spectrum += coef*light2.smooth;
							break;
						}
					}
				}
				color += (light.accurate * spectrum/sum).toColor();
			} else color += light.compute().toColor();

			color.r = pow(color.r, g);
			color.g = pow(color.g, g);
			color.b = pow(color.b, g);

			image->pixels[y][x] = color;
		}
	}
}
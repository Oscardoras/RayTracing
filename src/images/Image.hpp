#ifndef IMAGES_IMAGE_H_
#define IMAGES_IMAGE_H_

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "stbi.hpp"

#include "../algebra/Color.hpp"
#include "../MathUtils.cpp"


class Image {

public:

	int width;
	int height;
	Color **pixels;

	Image(int const& width, int const& height): width(width), height(height) {
		pixels = new Color *[height];
		for(int j = 0; j < height; j++) pixels[j] = new Color[width];
	}
	
	Image(Image const& image): width(image.width), height(image.height) {
		pixels = new Color *[height];
		for(int j = 0; j < height; j++) {
			pixels[j] = new Color[width];
			for(int i = 0; i < width; i++) pixels[j][i] = image.pixels[j][i];
		}
	}

	Image(std::string const& path) {
		const std::string ppm = ".ppm";
		if (path.size() > ppm.size() && std::equal(ppm.rbegin(), ppm.rend(), path.rbegin())) {
			std::ifstream stream(path);
			std::string line;
			std::getline(stream, line);

			std::getline(stream, line);
			int i = line.find_first_of(' ');
			width = std::stoi(line.substr(0, i));
			height = std::stoi(line.substr(i+1));
			pixels = new Color *[height];
			for(int j = 0; j < height; j++) pixels[j] = new Color[width];

			std::getline(stream, line);

			int n = 0;
			while (std::getline(stream, line)) {
				int y = n/width;
				int x = n%width;
				int i = line.find_first_of(' ');
				int j = line.find_last_of(' ');
				float r = std::stoi(line.substr(0, i))/255.;
				float g = std::stoi(line.substr(i+1, j-i-1))/255.;
				float b = std::stoi(line.substr(j+1))/255.;
				pixels[height-1-y][x] = Color(r, g, b);
				n++;
			}
		} else {
			const static int bytes_per_pixel = 3;
			unsigned char *data;
			int bytes_per_scanline;
			int components_per_pixel = bytes_per_pixel;

			data = stbi_load(path.c_str(), &width, &height, &components_per_pixel, components_per_pixel);
			if (!data) throw ("Could not load image file " + path);
			bytes_per_scanline = bytes_per_pixel * width;

			pixels = new Color *[height];
			for(int j = 0; j < height; j++) {
				pixels[j] = new Color[width];
				for(int i = 0; i < width; i++) {
					const float color_scale = 1.0 / 255.0;
					unsigned char *pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

					pixels[j][i] = Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
				}
			}
		}
	}

	~Image() {
		for(int j = 0; j < height; j++) delete[] pixels[j];
		delete[] pixels;
	}

	bool save(std::string const& path) {
		std::ofstream stream(path);
		if (stream) {
			stream << "P3\n" << width << ' ' << height << "\n255\n";
			for (int y = height-1; y >= 0; --y) for (int x = 0; x < width; ++x) {
				Color c = pixels[y][x];
				stream << static_cast<int>(255. * std::max(float(0), c.r)) << ' '
				<< static_cast<int>(255. * std::max(float(0), c.g)) << ' '
				<< static_cast<int>(255. * std::max(float(0), c.b)) << '\n';
			}
			return true;
		} else return false;
	}

	void filtre(std::vector<std::vector<float>> const& m) {
		Color **filtered = new Color *[height];
		for(int j = 0; j < height; j++) filtered[j] = new Color[width];

		int size = m.size();
		int s = (size-1)/2;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color c;
				for (int j = 0; j < size; j++) {
					for (int i = 0; i < size; i++) {
						if (y-s+j >= 0 && y-s+j <= height-1 && x-s+i >= 0 && x-s+i <= width-1) {
							c += m[j][i] * pixels[y-s+j][x-s+i];
						}
					}
				}
				filtered[y][x] = c;
			}
		}

		for(int j = 0; j < height; j++) delete[] pixels[j];
		delete[] pixels;
		pixels = filtered;
	}

	void multiply(float const& x) {
		filtre(std::vector<std::vector<float>> { std::vector<float> { x } });
	}

	void blur(int size) {
		std::vector<std::vector<float>> m;
		float value = 1/(size*size);
		for (int y = 0; y < size; y++) {
			std::vector<float> l;
			for (int x = 0; x < size; x++) {
				l.push_back(value);
			}
			m.push_back(l);
		}

		filtre(m);
	}

	void gaussian(int size, float o) {
		int s = (size-1)/2;
		std::vector<std::vector<float>> m;
		for (int y = -s; y <= s; y++) {
			std::vector<float> l;
			for (int x = -s; x <= s; x++) {
				l.push_back(std::exp( -(x*x + y*y) / (2*o*o) ) / (2*Pi*o*o));
			}
			m.push_back(l);
		}

		filtre(m);
	}

	void median(int size) {
		Color **filtered = new Color *[height];
		for(int j = 0; j < height; j++) filtered[j] = new Color[width];

		int s = (size-1)/2;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				std::vector<Color> v;

				for (int j = 0; j < size; j++) {
					for (int i = 0; i < size; i++) {
						if (y-s+j >= 0 && y-s+j <= height-1 && x-s+i >= 0 && x-s+i <= width-1) {
							v.push_back(pixels[y-s+j][x-s+i]);
						}
					}
				}
				
				filtered[y][x] = med(v);
			}
		}

		for(int j = 0; j < height; j++) delete[] pixels[j];
		delete[] pixels;
		pixels = filtered;
	}

	void maximize() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color& c = pixels[y][x];
				float max = 1.;
				if (c.r > max) max = c.r;
				if (c.g > max) max = c.g;
				if (c.b > max) max = c.b;
				if (max > 1.) c /= max;
			}
		}
	}

	void average() {
		Color color;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				color += pixels[y][x];
			}
		}
		color /= (width*height);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pixels[y][x] = color;
			}
		}
	}

	void contour_detection() {
		std::vector<std::vector<float>> m {
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., 24., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. }
		};

		filtre(m);
	}

};


#endif

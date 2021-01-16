#ifndef IMAGE_H_
#define IMAGE_H_

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "algebra/Color.hpp"


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
	}

	~Image() {
		for(int j = 0; j < height; j++) delete pixels[j];
		delete pixels;
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

		int s = m.size();
		int s2 = (s-1)/2;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color c;
				for (int j = 0; j < s; j++) {
					for (int i = 0; i < s; i++) {
						if (y-s2+j >= 0 && y-s2+j <= height-1 && x-s2+i >= 0 && x-s2+i <= width-1) {
							c += m[j][i] * pixels[y-s2+j][x-s2+i];
						}
					}
				}
				filtered[y][x] = c;
			}
		}

		for(int j = 0; j < height; j++) delete pixels[j];
		delete pixels;
		pixels = filtered;
	}

	void multiply(float x) {
		filtre(std::vector<std::vector<float>> { std::vector<float> { x } });
	}

	void blur3() {
		std::vector<std::vector<float>> m {
			std::vector<float> { 1./9., 1./9., 1./9. },
			std::vector<float> { 1./9., 1./9., 1./9. },
			std::vector<float> { 1./9., 1./9., 1./9. }
		};

		filtre(m);
	}

	void smooth3() {
		std::vector<std::vector<float>> m {
			std::vector<float> { 1./16., 2./16., 1./16. },
			std::vector<float> { 2./16., 4./16., 2./16. },
			std::vector<float> { 1./16., 2./16., 1./16. }
		};

		filtre(m);
	}

	void smooth5() {
		std::vector<std::vector<float>> m {
			std::vector<float> { 2./159., 4./159., 5./159., 4./159., 2./159. },
			std::vector<float> { 4./159., 9./159., 12./159., 9./159., 4./159. },
			std::vector<float> { 5./159., 12./159., 15./159., 12./159., 5./159. },
			std::vector<float> { 4./159., 9./159., 12./159., 9./159., 4./159. },
			std::vector<float> { 2./159., 4./159., 5./159., 4./159., 2./159. }
		};

		filtre(m);
	}

	void contour_detection5() {
		std::vector<std::vector<float>> m {
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., 24., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. },
			std::vector<float> { -1., -1., -1., -1., -1. }
		};

		filtre(m);
	}

	void gaussian(int size, float o) {
		int s = (size-1)/2;
		std::vector<std::vector<float>> m;
		for (int y = -s; y <= s; y++) {
			std::vector<float> l;
			for (int x = -s; x <= s; x++) {
				l.push_back(std::exp( -(x*x + y*y) / (2*o*o) ) / (2*pi*o*o));
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
				std::vector<float> vr;
				std::vector<float> vg;
				std::vector<float> vb;

				for (int j = 0; j < s; j++) {
					for (int i = 0; i < s; i++) {
						if (y-s+j >= 0 && y-s+j <= height-1 && x-s+i >= 0 && x-s+i <= width-1) {
							Color c = pixels[y-s+j][x-s+i];
							vr.push_back(c.r);
							vg.push_back(c.g);
							vb.push_back(c.b);
						}
					}
				}
				
				filtered[y][x] = Color(med(vr), med(vg), med(vb));
			}
		}

		for(int j = 0; j < height; j++) delete pixels[j];
		delete pixels;
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

};


#endif

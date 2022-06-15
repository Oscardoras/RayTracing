#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "stbi.hpp"

#include "Image.hpp"

#include "../MathUtils.hpp"



Matrix::Matrix(int const size): size(size) {
	elements = new float *[size];
	for (int i = 0; i < size; i++)
		elements[i] = new float[size];
}

Matrix::Matrix(int const size, float const values) {
	elements = new float *[size];
	for (int i = 0; i < size; i++)
		elements[i] = new float[size] {values};
}

Matrix::Matrix(Matrix const& matrix): size(matrix.size) {
	elements = new float *[size];
	for (int i = 0; i < size; i++) {
		elements[i] = new float[size];
		for (int j = 0; j < size; j++)
			elements[i][j] = matrix.elements[i][j];
	}
}

Matrix::~Matrix() {
	for(int i = 0; i < size; i++)
		delete[] elements[i];
	delete[] elements;
}

Matrix& Matrix::operator=(Matrix const& matrix) {
	for(int i = 0; i < size; i++)
		delete[] elements[i];
	delete[] elements;

	size = matrix.size;
	elements = new float *[size];
	for (int i = 0; i < size; i++) {
		elements[i] = new float[size];
		for (int j = 0; j < size; j++)
			elements[i][j] = matrix.elements[i][j];
	}

	return *this;
}


Image::Image(int const width, int const height): width(width), height(height) {
	pixels = new Color *[height];
	for (int i = 0; i < height; i++)
		pixels[i] = new Color[width];
}

Image::Image(Image const& image): width(image.width), height(image.height) {
	pixels = new Color *[height];
	for (int i = 0; i < height; i++) {
		pixels[i] = new Color[width];
		for (int j = 0; j < width; j++)
			pixels[i][j] = image.pixels[i][j];
	}
}

Image::Image(std::string const& path) {
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

Image::~Image() {
	for(int i = 0; i < height; i++)
		delete[] pixels[i];
	delete[] pixels;
}

Image& Image::operator=(Image const& image) {
	for(int i = 0; i < height; i++)
		delete[] pixels[i];
	delete[] pixels;

	width = image.width;
	height = image.height;
	pixels = new Color *[height];
	for (int i = 0; i < height; i++) {
		pixels[i] = new Color[width];
		for (int j = 0; j < width; j++)
			pixels[i][j] = image.pixels[i][j];
	}

	return *this;
}

bool Image::save(std::string const& path) const {
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

Image Image::filtre(Matrix const& m) const {
	Image filtered(width, height);
	int r = (m.size-1)/2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Color c;
			for (int j = 0; j < m.size; j++) {
				for (int i = 0; i < m.size; i++) {
					if (y-r+j >= 0 && y-r+j <= height-1 && x-r+i >= 0 && x-r+i <= width-1) {
						c += m.elements[j][i] * pixels[y-r+j][x-r+i];
					}
				}
			}
			filtered.pixels[y][x] = c;
		}
	}

	return filtered;
}

Image Image::multiply(float const x) const {
	return filtre(Matrix(1, x));
}

Image Image::blur(int const size) const {
	return filtre(Matrix(size, 1./(size*size)));
}

Image Image::gaussian(int const size, float const sigma) const {
	Matrix m(size);
	int r = (size-1)/2;
	for (int y = -r; y <= r; y++)
		for (int x = -r; x <= r; x++)
			m.elements[x+r][y+r] = std::exp( -(x*x + y*y) / (2*sigma*sigma) ) / (2*Pi*sigma*sigma);

	return filtre(m);
}

Image Image::contour_detection() const {
	Matrix m(5, 1.);
	m.elements[2][2] = 24.;
	return filtre(m);
}

void Image::maximize() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Color & c = pixels[y][x];
			float max = 1.;
			if (c.r > max) max = c.r;
			if (c.g > max) max = c.g;
			if (c.b > max) max = c.b;
			if (max > 1.) c /= max;
		}
	}
}

Color Image::getAverage() const {
	Color color;

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			color += pixels[y][x];

	return color / (width*height);
}
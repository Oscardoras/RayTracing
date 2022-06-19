#ifndef __IMAGES_IMAGE_H__
#define __IMAGES_IMAGE_H__

#include <string>

#include "../algebra/Spectrum.hpp"


struct Matrix {

	int size;
	float **elements;

	Matrix(int const size);
	Matrix(int const size, float const values);
	Matrix(Matrix const& matrix);

	~Matrix();

	Matrix& operator=(Matrix const& matrix);

};

struct Image {

	int width;
	int height;
	Color **pixels;

	Image(int const width, int const height);
	Image(Image const& image);
	Image(std::string const& path);
	~Image();

	Image& operator=(Image const& image);

	bool save(std::string const& path) const;

	Image filtre(Matrix const& m) const;
	Image multiply(float const x) const;
	Image blur(int const radius) const;
	Image gaussian(int const radius, float const sigma) const;
	Image contour_detection() const;
	void maximize();

	Color getAverage() const;

};


#endif

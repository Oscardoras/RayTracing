#ifndef TEXTURES_IMAGETEXTURE_H_
#define TEXTURES_IMAGETEXTURE_H_

#include <memory>
#include <algorithm>
#include "../libimage.hpp"

#include "Texture.hpp"
#include "../Image.hpp"


inline double clamp(double x, double min, double max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

class ImageTexture: public Texture {

public:

	unsigned char *data;
	int t_width;
	int t_height;
	int bytes_per_scanline;

	float width;
	float height;

	const static int bytes_per_pixel = 3;

	ImageTexture(const char* path, float const& width, float const& height): Texture(), width(width), height(height) {
		int components_per_pixel = bytes_per_pixel;

		data = stbi_load(path, &t_width, &t_height, &components_per_pixel, components_per_pixel);

		if (!data) {
			std::cerr << "ERROR: Could not load texture image file '" << path << "'.\n";
			t_width = t_width = 0;
		}

		bytes_per_scanline = bytes_per_pixel * width;
	}

	~ImageTexture() {
		delete data;
	}

	virtual Color get(float const& u, float const& v) const override {
		/*// Clamp input texture coordinates to [0,1] x [1,0]
		float u1 = u/width;
		float v1 = v/height;  // Flip V to image coordinates

		int i = int(u1 * t_width);
		int j = int(v1 * t_height);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i < 0)  i = 0;
		if (j < 0) j = 0;
		if (i >= t_width)  i = t_width-1;
		if (j >= t_height) j = t_height-1;

		const double color_scale = 1.0 / 255.0;
		int p = j*bytes_per_scanline + i*bytes_per_scanline;
		unsigned char *pixel = data + j*bytes_per_scanline + i*bytes_per_scanline;

		return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);*/
		// If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return Color(1,0,0);

            // Clamp input texture coordinates to [0,1] x [1,0]
            double u1 = clamp(u, 0.0, 1.);
            double v1 = 1.0 - clamp(v, 0.0, 1.);  // Flip V to image coordinates

            auto i = static_cast<int>(u1 * t_width);
            auto j = static_cast<int>(v1 * t_height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= t_width)  i = t_width-1;
            if (j >= t_height) j = t_height-1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

            return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
	}

};

class image_texture : public Texture {
    public:
        const static int bytes_per_pixel = 3;

        image_texture()
          : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        image_texture(const char* filename) {
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~image_texture() {
            delete data;
        }

        virtual Color get(float const& u0, float const& v0) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return Color(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            auto u = clamp(u0, 0.0, 1.0);
            auto v = 1.0 - clamp(v0, 0.0, 1.0);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width-1;
            if (j >= height) j = height-1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

            return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
};


#endif

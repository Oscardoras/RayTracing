#ifndef IMAGES_RENDERING_H_
#define IMAGES_RENDERING_H_

#include "Image.hpp"
#include "../materials/Material.hpp"


class Rendering {

public:

	int width;
	int height;
	Image scatterAlbedo;
	Image scatterIllumination;
	Image addition;
	long **ids;

	Rendering(int const& width, int const& height): width(width), height(height), scatterAlbedo(width, height), scatterIllumination(width, height), addition(width, height) {
		ids = new long *[height];
		for(int j = 0; j < height; j++) ids[j] = new long [width];
	}

	Rendering(Rendering const& rendering): width(rendering.width), height(rendering.height), scatterAlbedo(rendering.scatterAlbedo), scatterIllumination(rendering.scatterIllumination), addition(rendering.addition) {
		ids = new long *[height];
		for(int j = 0; j < height; j++) {
			ids[j] = new long [width];
			for(int i = 0; i < width; i++) ids[j][i] = rendering.ids[j][i];
		}
	}

	~Rendering() {
		for(int j = 0; j < height; j++) delete[] ids[j];
		delete[] ids;
	}

	void set(int const& x, int const& y, Light const& light) {
		scatterAlbedo.pixels[y][x] = light.scatterAlbedo.toColor();
		scatterIllumination.pixels[y][x] = light.scatterIllumination.toColor();
		addition.pixels[y][x] = light.addition.toColor();
		ids[y][x] = light.id;
	}

	void homogenize(int size, float o) {
		Color **filtered = new Color *[height];
		for(int j = 0; j < height; j++) filtered[j] = new Color[width];
		
		for (int y = 0; y < height; y++) {
			std::cout << "Homogenizing line : " << y << std::endl;
			for (int x = 0; x < width; x++) {
				long id = ids[y][x];
				if (id != 0) {
					Color c;
					float sum = 0.;
					
					for (int j = -size; j <= size; j += 2) {
						for (int i = -size; i <= size; i += 2) {
							if (y+j >= 0 && y+j <= height-1 && x+i >= 0 && x+i <= width-1) {
								if (ids[y+j][x+i] == id) {
									float coef = std::exp( -(i*i + j*j) / (2*o*o) ) / (2*pi*o*o);
									sum += coef;
									c += coef*scatterIllumination.pixels[y+j][x+i];
								}
							}
						}
					}

					filtered[y][x] = c/sum;
				} else filtered[y][x] = scatterIllumination.pixels[y][x];
			}
		}

		for(int j = 0; j < height; j++) delete[] scatterIllumination.pixels[j];
		delete[] scatterIllumination.pixels;
		scatterIllumination.pixels = filtered;
	}

	Image render(float const& gamma) {
		Image image(width, height);
		float g = 1/gamma;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color color = scatterIllumination.pixels[y][x]*scatterAlbedo.pixels[y][x] + addition.pixels[y][x];
				color.r = pow(color.r, g);
				color.g = pow(color.g, g);
				color.b = pow(color.b, g);
				image.pixels[y][x] = color;
			}
		}
		return image;
	}

};


#endif

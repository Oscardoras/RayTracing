#ifndef RENDERING_H_
#define RENDERING_H_

#include "Image.hpp"
#include "materials/Material.hpp"


class Rendering {

public:

	int width;
	int height;
	Image albedo;
	Image light;
	long **ids;

	Rendering(int const& width, int const& height): width(width), height(height), albedo(width, height), light(width, height) {
		ids = new long *[height];
		for(int j = 0; j < height; j++) ids[j] = new long [width];
	}
	
	Rendering(Image const& albedo, Image const& light): width(albedo.width), height(albedo.height), albedo(albedo), light(light) {
		ids = new long *[height];
		for(int j = 0; j < height; j++) ids[j] = new long [width];
	}

	Rendering(Rendering const& rendering): width(rendering.width), height(rendering.height), albedo(rendering.albedo), light(rendering.light) {
		ids = new long *[height];
		for(int j = 0; j < height; j++) {
			ids[j] = new long [width];
			for(int i = 0; i < width; i++) ids[j][i] = rendering.ids[j][i];
		}
	}

	~Rendering() {
		for(int j = 0; j < height; j++) delete ids[j];
		delete ids;
	}

	void set(int const& x, int const& y, LightData const& lightData) {
		albedo.pixels[y][x] = lightData.albedo;
		light.pixels[y][x] = lightData.light;
		ids[y][x] = lightData.id;
	}

	void homogenize(int size, float o) {
		Color **filtered = new Color *[height];
		for(int j = 0; j < height; j++) filtered[j] = new Color[width];

		int s = 1+2*size;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				long id = ids[y][x];
				if (id != 0) {
					Color c;
					float sum = 0.;
					
					for (int j = -size; j <= size; j++) {
						for (int i = -size; i <= size; i++) {
							if (y+j >= 0 && y+j <= height-1 && x+i >= 0 && x+i <= width-1) {
								if (ids[y+j][x+i] == id) {
									float coef = std::exp( -(i*i + j*j) / (2*o*o) ) / (2*pi*o*o);
									sum += coef;
									c += coef*light.pixels[y+j][x+i];
								}
							}
						}
					}

					filtered[y][x] = c/sum;
				} else filtered[y][x] = light.pixels[y][x];
			}
		}

		for(int j = 0; j < height; j++) delete light.pixels[j];
		delete light.pixels;
		light.pixels = filtered;
	}

	Image render(float const& gamma) {
		Image image(width, height);
		float g = 1/gamma;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color color = light.pixels[y][x]*albedo.pixels[y][x];
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

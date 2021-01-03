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
	Material ***materials;

	Rendering(int const& width, int const& height): width(width), height(height), albedo(width, height), light(width, height) {
		materials = new Material **[height];
		for(int j = 0; j < height; j++) materials[j] = new Material *[width];
	}
	
	Rendering(Image const& albedo, Image const& light): width(albedo.width), height(albedo.height), albedo(albedo), light(light) {
		materials = new Material **[height];
		for(int j = 0; j < height; j++) materials[j] = new Material *[width];
	}

	Rendering(Rendering const& rendering): width(rendering.width), height(rendering.height), albedo(rendering.albedo), light(rendering.light) {
		materials = new Material **[height];
		for(int j = 0; j < height; j++) {
			materials[j] = new Material *[width];
			for(int i = 0; i < width; i++) materials[j][i] = rendering.materials[j][i];
		}
	}

	~Rendering() {
		for(int j = 0; j < height; j++) delete materials[j];
		delete materials;
	}

	void set(int const& x, int const& y, LightData const& lightData) {
		albedo.pixels[y][x] = lightData.albedo;
		light.pixels[y][x] = lightData.light;
		materials[y][x] = lightData.material;
	}

	void homogenize(int size) {
		Color **filtered = new Color *[height];
		for(int j = 0; j < height; j++) filtered[j] = new Color[width];
		
		int s = 1+2*size;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color c;
				int n = 0;
				Material *material = materials[y][x];
				for (int j = 0; j < s; j++) {
					for (int i = 0; i < s; i++) {
						if (y-size+j >= 0 && y-size+j <= height-1 && x-size+i >= 0 && x-size+i <= width-1) {
							if (materials[y-size+j][x-size+i] == material) {
								c += light.pixels[y-size+j][x-size+i];
								n++;
							}
						}
					}
				}
				filtered[y][x] = c/n;
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

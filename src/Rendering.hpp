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

	void homogenize(int median, int blur) {
		Color **medianed = new Color *[height];
		for(int j = 0; j < height; j++) medianed[j] = new Color[width];

		int m = 1+2*median;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				long id = ids[y][x];
				if (id != 0) {
					std::vector<float> vr;
					std::vector<float> vg;
					std::vector<float> vb;

					for (int j = 0; j < m; j++) {
						for (int i = 0; i < m; i++) {
							if (y-median+j >= 0 && y-median+j <= height-1 && x-median+i >= 0 && x-median+i <= width-1) {
								if (ids[y-median+j][x-median+i] == id) {
									Color c = light.pixels[y-median+j][x-median+i];
									vr.push_back(c.r);
									vg.push_back(c.g);
									vb.push_back(c.b);
								}
							}
						}
					}
					
					medianed[y][x] = Color(med(vr), med(vg), med(vb));
				} else medianed[y][x] = light.pixels[y][x];
			}
		}

		Color **blured = new Color *[height];
		for(int j = 0; j < height; j++) blured[j] = new Color[width];

		int b = 1+2*blur;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				long id = ids[y][x];
				if (id != 0) {
					Color c;
					int n = 0;
					for (int j = 0; j < b; j++) {
						for (int i = 0; i < b; i++) {
							if (y-blur+j >= 0 && y-blur+j <= height-1 && x-blur+i >= 0 && x-blur+i <= width-1) {
								if (ids[y-blur+j][x-blur+i] == id) {
									c += medianed[y-blur+j][x-blur+i];
									n++;
								}
							}
						}
					}
					blured[y][x] = c/n;
				} else blured[y][x] = medianed[y][x];
			}
		}

		for(int j = 0; j < height; j++) delete light.pixels[j];
		delete light.pixels;
		for(int j = 0; j < height; j++) delete medianed[j];
		delete medianed;
		light.pixels = blured;
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

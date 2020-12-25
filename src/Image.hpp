#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>

#include "algebra/Color.hpp"


class Image {

public:

	int width;
	int height;
	Color **pixels;

	Image(int width, int height):
		width(width), height(height) {
			pixels = new Color *[height];
			for(int i = 0; i <height; i++) pixels[i] = new Color[width];
		}
	
	Color get(int x, int y) {
		return pixels[y][x];
	}
	
	void set(int x, int y, Color color) {
		pixels[y][x] = color;
	}

	void smooth(int r) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int n = 1;
				Color c = pixels[y][x];
				for (int j = std::max(0, y-r); j <= std::min(y+r, height-1); j++) {
					for (int i = std::max(0, x-r); i <= std::min(x+r, width-1); i++) {
						c += pixels[j][i];
						n++;
					}
				}
				pixels[y][x] = c/n;
			}
		}
	}

};


#endif

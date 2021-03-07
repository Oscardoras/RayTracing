#ifndef IMAGES_RENDERING_H_
#define IMAGES_RENDERING_H_

#include "Image.hpp"
#include "../materials/Material.hpp"


class Rendering {

public:

	Image accurate;
	std::vector<Scatter> **smooth;

	Rendering(int const& width, int const& height): accurate(width, height) {
		smooth = new std::vector<Scatter> *[height];
		for(int j = 0; j < height; j++) smooth[j] = new std::vector<Scatter> [width];
	}

	Rendering(Rendering const& rendering): accurate(rendering.accurate) {
		int const& width = rendering.accurate.width;
		int const& height = rendering.accurate.height;

		smooth = new std::vector<Scatter> *[height];
		for(int j = 0; j < height; j++) {
			smooth[j] = new std::vector<Scatter> [width];
			for(int i = 0; i < width; i++) smooth[j][i] = rendering.smooth[j][i];
		}
	}

	~Rendering() {
		for(int j = 0; j < accurate.height; j++) delete[] smooth[j];
		delete[] smooth;
	}

	void set(int const& x, int const& y, Light const& light) {
		accurate.pixels[y][x] = light.accurate.toColor();
		smooth[y][x] = light.smooth;
	}

private:

	static void threadRender(std::mutex *lock, std::vector<int> *lines, Rendering *rendering, Image *image, int size, float g, std::vector<std::vector<float>> m) {
		int const& width = rendering->accurate.width;
		int const& height = rendering->accurate.height;

		while (!lines->empty()) {
			lock->lock();
			int y;
			if (!lines->empty()) {
				y = lines->front();
				std::cout << "Homogenizing line : " << y << std::endl;
				lines->erase(lines->begin());
			} else break;
			lock->unlock();

			for (int x = 0; x < width; x++) {
				Color color = rendering->accurate.pixels[y][x];
				for (Scatter& scatter : rendering->smooth[y][x]) {
					long const& id = scatter.id;
					Spectrum spectrum;
					float sum = 0.;

					for (int j = -size; j <= size; j++) {
						for (int i = -size; i <= size; i++) {
							if (y+j >= 0 && y+j <= height-1 && x+i >= 0 && x+i <= width-1) {
								for (Scatter s : rendering->smooth[y+j][x+i]) if (s.id == id) {
									float coef = m[size + j][size + i];
									sum += coef;
									spectrum += coef*s.scattered;
									break;
								}
							}
						}
					}
					color += (scatter.albedo * spectrum/sum).toColor();
				}
				color.r = pow(color.r, g);
				color.g = pow(color.g, g);
				color.b = pow(color.b, g);
				lock->lock();
				image->pixels[y][x] = color;
				lock->unlock();
			}
		}
	}

public:

	Image render(float const& gamma, int const& size, float const& o, int const& t) {
		int const& width = accurate.width;
		int const& height = accurate.height;

		Image image = Image(width, height);

		float g = 1/gamma;

		std::vector<std::vector<float>> m;
		for (int j = -size; j <= size; j++) {
			std::vector<float> l;
			for (int i = -size; i <= size; i++) {
				l.push_back(std::exp( -(i*i + j*j) / (2*o*o) ) / (2*Pi*o*o));
			}
			m.push_back(l);
		}
		
		std::vector<int> lines = std::vector<int>();
		for (int i = height-1; i >= 0; i--) lines.push_back(i);

		std::vector<std::shared_ptr<std::thread>> threads;
		std::mutex lock;

		for (int i = 0; i < t; i++) {
			threads.push_back(std::make_shared<std::thread>(Rendering::threadRender, &lock, &lines, this, &image, size, g, m));
			//Rendering::threadRender(&lock, &lines, this, &image, size, g, m);
		}
		for (int i = 0; i < threads.size(); i++) threads[i]->join();

		return image;
	}

	Image render(float const& gamma) {
		int const& width = accurate.width;
		int const& height = accurate.height;
		float g = 1/gamma;

		Image image = Image(width, height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color color = accurate.pixels[y][x];
				for (Scatter scatter : smooth[y][x]) color += (scatter.albedo * scatter.scattered).toColor();
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

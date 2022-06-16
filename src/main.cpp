#include <iostream>
#include <memory>
#include <ctime>

#include "materials/models/Lamp.hpp"
#include "materials/models/Lambertian.hpp"

#include "objects/primitives/Sphere.hpp"

#include "textures/Plain.hpp"

#include "world/SimpleWorld.hpp"

#include "Camera.hpp"


struct Level: public SimpleWorld {

	virtual Spectrum infiniteSpectrum(Ray const& r) const override {
		Vector unit_direction = r.v.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		//return ((1.0-t)*Spectrum(1.0, 1.0, 1.0) + t*Spectrum(0.5, 0.7, 1.0));
		return Spectrum();
	}

	virtual Spectrum maxDepthSpectrum(Ray const& r) const override {
		return Spectrum();
	}

};


int main() {
	Level level;
	Plain plain(Spectrum(1,0,0));
	Lamp lamp(&plain);
	Sphere sphere(Point (5,0,0), 1, &lamp);
	level.objects.push_back(&sphere);

	Plain plain2(Spectrum(0.5,0.5,0.5));
	Lambertian lamp2(&plain2);
	Sphere sphere2(Point (5,-50,0), 49, &lamp2);
	level.objects.push_back(&sphere2);

	Camera camera(&level, 480, 16./9., 2.);
	camera.setPosition(Point(0,0,0), Vector(1,0,0), Vector(0,1,0));

	std::time_t t0 = std::time(nullptr);
	auto image = camera.render(1, 0.8, 10, 50);
	std::cout << level.nbr << " in " << std::time(nullptr) - t0 << " seconds." << std::endl;

	image = image.gaussian(5, 0.8);
	image.save("image.ppm");

	return 0;
}

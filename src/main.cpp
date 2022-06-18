#include <iostream>
#include <memory>
#include <ctime>

#include "materials/models/Lambertian.hpp"
#include "materials/models/Lamp.hpp"
#include "materials/models/Metal.hpp"
#include "materials/models/Phong.hpp"

#include "objects/primitives/Sphere.hpp"

#include "textures/Plain.hpp"

#include "world/SimpleWorld.hpp"

#include "Camera.hpp"


struct Level: public SimpleWorld {

	virtual Spectrum infiniteSpectrum(Ray const& r) const override {
		Vector unit_direction = r.v.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return ((1.0-t)*Spectrum(1.0, 1.0, 1.0) + t*Spectrum(0.5, 0.7, 1.0));
		return Spectrum();
	}

	virtual Spectrum maxDepthSpectrum(Ray const& r) const override {
		return Spectrum();
	}

};


int main() {
	Level level;
	Plain floor_texture(Spectrum(0.5,0.5,0.5));
	Lambertian floor_material(&floor_texture);
	Sphere floor(Point (5,-100,0), 100, &floor_material);
	level.objects.push_back(&floor);

/*
	Plain red_texture(Spectrum(1,0,0));
	Lamp lamp1(&red_texture);
	Sphere sphere1(Point (5,1,-2), 1, &lamp1);
	level.objects.push_back(&sphere1);

	Plain green_texture(Spectrum(0,1,0));
	Lamp lamp2(&green_texture);
	Sphere sphere2(Point (5,1,2), 1, &lamp2);
	level.objects.push_back(&sphere2);

	Plain ambiant_texture(Spectrum(0.2,0,0));
	Plain diffuse_texture(Spectrum(0.8,0,0));
	Plain specular_texture(Spectrum(1,1,1));
	Plain alpha_texture(Spectrum::white()*100);
	Phong phong1(&ambiant_texture, &diffuse_texture, &specular_texture, &alpha_texture, std::vector<LightSource> {LightSource(Vector(0,1,0))});
	Sphere sphere3(Point (5,1,0), 1, &phong1);
	level.objects.push_back(&sphere3);
*/
	Plain metal_texture(Spectrum(1,0,0));
	Plain roughness_texture(Spectrum(0.2,0.2,0.2));
	Metal metal1(&metal_texture, &roughness_texture);
	Sphere sphere4(Point (5,1,0), 1, &metal1);
	level.objects.push_back(&sphere4);

	Camera camera(&level, 480, 16./9., 2.);
	camera.setPosition(Point(0,1,0), Vector(1,0,0), Vector(0,1,0));

	std::time_t t0 = std::time(nullptr);
	auto image = camera.render(10, 0.8, 20, 10);
	std::cout << *level.nbr << " in " << std::time(nullptr) - t0 << " seconds." << std::endl;

	image = image.gaussian(5, 0.8*2);
	image.save("image.ppm");

	return 0;
}

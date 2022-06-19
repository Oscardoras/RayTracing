#include <iostream>
#include <memory>
#include <chrono>

#include "materials/models/Lambertian.hpp"
#include "materials/models/Dielectric.hpp"
#include "materials/models/Lamp.hpp"
#include "materials/models/Metal.hpp"
#include "materials/models/Phong.hpp"

#include "objects/primitives/Sphere.hpp"
#include "objects/primitives/Triangle.hpp"

#include "textures/ImageTexture.hpp"
#include "textures/Plain.hpp"
#include "textures/Tile.hpp"

#include "world/SimpleWorld.hpp"

#include "Camera.hpp"


struct Level: public SimpleWorld {

	virtual Light infiniteLight(Ray const& r) const override {
		Vector unit_direction = r.v.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return Light(((1.0-t)*Spectrum(1.0, 1.0, 1.0) + t*Spectrum(0.5, 0.7, 1.0)));
		return Light(Spectrum());
	}

	virtual Light maxDepthLight(Ray const& r) const override {
		return Light(Spectrum());
	}

};


int main() {
	Level level;
	Plain floor_texture(Spectrum(0.5,0.5,0.5));
	Lambertian floor_material(&floor_texture);
	Sphere floor(Point (5,-100,0), 100, &floor_material);
	level.primitives.push_back(&floor);

/*
	Plain red_texture(Spectrum(1,0,0));
	Lamp lamp1(&red_texture);
	Sphere sphere1(Point(5,1,-2), 1, &lamp1);
	level.primitives.push_back(&sphere1);

	Plain green_texture(Spectrum(0,1,0));
	Lamp lamp2(&green_texture);
	Sphere sphere2(Point(5,1,2), 1, &lamp2);
	level.primitives.push_back(&sphere2);
*/
/*
	Plain ambiant_texture(Spectrum(0.2,0,0));
	Plain diffuse_texture(Spectrum(0.8,0,0));
	Plain specular_texture(Spectrum(1,1,1));
	Plain alpha_texture(Spectrum::white()*100);
	Phong phong1(&ambiant_texture, &diffuse_texture, &specular_texture, &alpha_texture, std::vector<LightSource> {LightSource(Vector(0,1,0))});
	Sphere sphere3(Point(5,1,0), 1, &phong1);
	level.primitives.push_back(&sphere3);
*/
/*
	Plain metal_texture(Spectrum(1,0,0));
	Plain roughness_texture(Spectrum(0.2,0.2,0.2));
	Metal metal1(&metal_texture, &roughness_texture);
	Sphere sphere4(Point(5,1,0), 1, &metal1);
	level.primitives.push_back(&sphere4);
*/

	Plain roughness_texture(Spectrum(0,0,0));
	Dielectric glass1(1.5f, &roughness_texture);
	Sphere sphere5(Point(5,1,0), 1, &glass1);
	level.primitives.push_back(&sphere5);

/*
	Image earth("images/textures/earthmap.jpg");
	ImageTexture triangle_texture(&earth, 4, 2);
	Lambertian triangle_material(&triangle_texture);
	Triangle triangle(Point(5,1,-2), Point(5,1,2), Point(5,3,1.95), &triangle_material);
	level.primitives.push_back(&triangle);
*/


	Camera camera(&level, 1080, 16./9., 2.);
	camera.setPosition(Point(0,1,0), Vector(1,0,0), Vector(0,1,0));

	auto start = std::chrono::high_resolution_clock::now();
	Image image = camera.render(20, 0.8, 5, 10);
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << *level.nbr << " in " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " milliseconds." << std::endl;

	//image = image.gaussian(2, 0.8*2);
	image.save("image.ppm");

	return 0;
}

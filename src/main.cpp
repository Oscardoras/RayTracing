#include <iostream>
#include <fstream>
#include <memory>

#include "Camera.hpp"
#include "hittables/Sphere.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Metal.hpp"
#include "materials/Dielectric.hpp"
#include "materials/Light.hpp"


class Level: public World {

public:

	Color infiniteColor(Ray const& r) const {
		Vector unit_direction = r.direction.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
		//return Color();
	}

};

std::shared_ptr<World> getLevel() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5))));
	level->add(std::make_shared<Sphere>(Point(-1,0,0.5), 0.5, std::make_shared<Lambertian>(Color(1., 1., 0.))));
	level->add(std::make_shared<Sphere>(Point(1,0,0.5), 0.5, std::make_shared<Metal>(Color(1., 1., 1.), 0.)));
	//level->add(std::make_shared<Sphere>(Point(0,0,-1), 0.5, std::make_shared<Light>(Color(5., 5., 5.))));
	return level;
}

int main() {
	std::ofstream stream("image.ppm");
	if (stream) {
		Point pos = Point(0., 0., 2.);
		Camera cam(getLevel(), pos, (Point(1,0,0.5) - pos).unit(), Vector(0,1,0), 1080, 16./9., 2., 30, 20);

		Image image = cam.render();
		image.smooth(1);

		stream << "P3\n" << image.width << ' ' << image.height << "\n255\n";
		for (int y = image.height-1; y >= 0; --y) for (int x = 0; x < image.width; ++x) writeColor(stream, image.get(x, y));

	} else {
		std::cerr << "File error" << std::endl;
	}
	return 0;
}

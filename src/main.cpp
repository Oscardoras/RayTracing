#include <iostream>
#include <fstream>
#include <memory>

#include "algebra/Color.h"
#include "algebra/Point.h"
#include "algebra/Ray.h"
#include "algebra/Vector.h"
#include "Camera.h"
#include "hittables/Sphere.h"
# include "materials/Lambertian.h"
# include "materials/Metal.h"
# include "materials/Light.h"


class Level: public World {

public:

	Color infiniteColor(Ray const& r) const {
		Vector unit_direction = r.direction.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		//return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(1.0, 0.8, 0.);
		return Color();
	}

};

std::shared_ptr<World> getLevel() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(Color(1., 1., 1.))));
	level->add(std::make_shared<Sphere>(Point(1,0,0.5), 0.5, std::make_shared<Metal>(Color(1., 1., 1.))));
	level->add(std::make_shared<Sphere>(Point(0,0,0.5), 0.5, std::make_shared<Light>(Color(1., 1., 1.))));
	return level;
}

int main() {
	std::ofstream stream("image.ppm");
	if (stream) {
		Camera cam(getLevel(), Point(0., 0., 2.), (Point(0,0,-1) - Point(0., 0., 1.)), 480, 16./9., 1.0, 15, 50);
		cam.render(stream);

	} else {
		std::cerr << "File error" << std::endl;
	}
	return 0;
}

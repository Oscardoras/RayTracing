#include <iostream>
#include <fstream>
#include <memory>

#include "algebra/Color.h"
#include "algebra/Point.h"
#include "algebra/Ray.h"
#include "algebra/Vector.h"
#include "Camera.h"
#include "hittables/Sphere.h"
#include "Window.h"


class Level: public World {

public:

	Color infiniteColor(Ray const& r) const {
		Vector unit_direction = r.direction.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
	}

};

std::shared_ptr<World> getLevel() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,0,-1), 0.5));
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100));
	return level;
}

int main() {
	Window_main();

	std::ofstream stream("image.ppm");
	if (stream) {

		Camera cam(getLevel(), Point(0., 0., 1.), Vector(0., 0., 1.), 1080, 16./9., 1.0, 10);
		cam.render(stream);

	} else {
		std::cerr << "File error" << std::endl;
	}
	return 0;
}

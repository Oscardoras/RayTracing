#include <iostream>
#include <memory>

int n = 0;

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
		return Color();
	}

};

std::shared_ptr<World> getLevel() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5))));
	level->add(std::make_shared<Sphere>(Point(-1,0,0.5), 0.5, std::make_shared<Lambertian>(Color(1., 1., 0.))));
	level->add(std::make_shared<Sphere>(Point(1,0,0.5), 0.5, std::make_shared<Metal>(Color(1., 1., 1.), 0.)));
	//level->add(std::make_shared<Sphere>(Point(0,0,-1), 0.5, std::make_shared<Light>(10.*Color(1., 1., 1.))));
	//level->priorities.push_back(std::make_shared<Priority>(Point(0,0,-1), 0.5));
	return level;
}

std::shared_ptr<World> getLevel2() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5))));
	
	for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color(1,0,0);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    level->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // metal
                    auto albedo = Color(0,1,0);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    level->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

	return level;
}

int main() {
	int maxRayPerPixel = 15;
	Point pos = Point(4., 2, -2.);
	Camera cam(getLevel(), pos, (Point(1,0,0.5) - pos).unit(), Vector(0,1,0), 720, 16./9.);
	//cam.world->priority = 0.9;

	Rendering rendering = cam.render(4, true, maxRayPerPixel, 20);

	rendering.light.save("raw_light.ppm");
	rendering.render(2.).save("raw.ppm");
	std::cout << n << " rays" << std::endl;

	rendering.homogenize(2, 5);

	rendering.albedo.save("albedo.ppm");
	rendering.light.save("light.ppm");

	Image image = rendering.render(2.);
	image.smooth3();
	image.save("image.ppm");

	Image contour(image);
	contour.contour_detection5();
	contour.save("image_contour.ppm");

	return 0;
}

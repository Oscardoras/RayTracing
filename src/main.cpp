#include <iostream>
#include <memory>
#include <ctime>

int nbr = 0;

#include "Camera.hpp"
#include "objects/Sphere.hpp"
#include "objects/Triangle.hpp"
#include "materials/Lambertian.hpp"
//#include "materials/Metal.hpp"
//#include "materials/Dielectric.hpp"
//#include "materials/Lamp.hpp"
//#include "materials/Marble.hpp"
//#include "materials/Fog.hpp"
#include "textures/Plain.hpp"
#include "textures/Tile.hpp"
#include "textures/ImageTexture.hpp"


class Level: public World {

public:

	Spectrum infiniteColor(Ray const& r) const {
		Vector unit_direction = r.direction.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return (1.0-t)*Spectrum(1.0, 1.0, 1.0) + t*Spectrum(0.5, 0.7, 1.0);
		return Spectrum();
	}

};

std::shared_ptr<World> getLevel() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(std::make_shared<Plain>(0.5, 0.5, 0.5))));

	//level->add(std::make_shared<Rectangle>(-3, 0, 2, -2, 2, std::make_shared<Metal>(Color(1, 1, 1), 0.)));
	//level->add(std::make_shared<Rectangle>(-2.01, -0.1, 2.1, -2.1, 2.1, std::make_shared<Lambertian>(std::make_shared<Noise>(4))));
	//level->add(std::make_shared<Triangle>(Point(-2,3,-2), Point(2,0,-2), Point(2,0,3), std::make_shared<Marble>(Point(), Vector(1,0,-1), 7)));
	//level->add(std::make_shared<Sphere>(Point(1,0,0.5), 0.5, std::make_shared<Lambertian>(std::make_shared<ImageTexture>(Image("earthmap.jpg"), pi, pi/2))));
	//level->add(std::make_shared<Sphere>(Point(1,0,0.5), 1, std::make_shared<Fog>(Spectrum(1,1,1), 0.5)));
	//level->add(std::make_shared<Sphere>(Point(1,0,-1), 0.5, std::make_shared<Dielectric>(Color(1., 1., 1.), 1.5)));
	//level->add(std::make_shared<Sphere>(Point(-1,0,-1), 0.5, std::make_shared<Textured>()));
	//level->add(std::make_shared<Sphere>(Point(0,0,100), 1, std::make_shared<Light>(25000.*Color(1., 1., 1.))));
	//level->priorities.push_back(std::make_shared<Priority>(Point(0,0,100), 1));

	level->sort(false);
	//level->tree->print();
	return level;
}

/*std::shared_ptr<World> getLevel2() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(std::make_shared<Plain>(0.5, 0.5, 0.5))));
	
	for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color(1,0,0);
                    sphere_material = std::make_shared<Lambertian>(std::make_shared<Plain>(albedo));
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

	level->sort();
	level->tree->print();
	return level;
}*/

int main() {
	int maxRayPerPixel = 5;
	Point pos = Point(-4, 1, 4);
	//Point pos = Point(800, 5, 10);
	Camera cam(getLevel(), pos, (Point(1,0,0.5) - pos).unit(), Vector(0,1,0), 720, 16./9.);
	//cam.world->priority = 1.0;

	std::time_t t0 = std::time(nullptr);
	Rendering rendering = cam.render(4, false, maxRayPerPixel, 20);
	std::cout << std::time(nullptr) - t0 << " seconds" << std::endl;

	rendering.scatterIllumination.save("images/raw_illumination.ppm");
	rendering.scatterAlbedo.save("images/albedo.ppm");
	rendering.addition.save("images/addition.ppm");
	rendering.render(2.).save("images/raw.ppm");
	std::cout << nbr << " rays" << std::endl;

	//rendering.illumination.maximize();
	rendering.homogenize(8, 6.);

	rendering.scatterIllumination.save("images/illumination.ppm");

	Image image = rendering.render(2.);
	image.gaussian(3, 0.8);
	image.save("images/image.ppm");

	Image contour(image);
	contour.contour_detection();
	contour.save("images/image_contour.ppm");

	return 0;
}

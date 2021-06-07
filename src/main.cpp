#include <iostream>
#include <memory>
#include <ctime>

int nbr = 0;

#include "Camera.hpp"
#include "objects/primitives/Sphere.hpp"
#include "objects/primitives/Triangle.hpp"
#include "objects/Rectangle.hpp"
#include "objects/Parallelepiped.hpp"
#include "materials/models/Lambertian.hpp"
#include "materials/models/Metal.hpp"
#include "materials/models/Lamp.hpp"
#include "materials/models/Dielectric.hpp"
#include "materials/BDF.hpp"
#include "materials/matter/Marble.hpp"
#include "materials/matter/Wood.hpp"
#include "materials/matter/Brick.hpp"
#include "materials/matter/Gold.hpp"
#include "materials/matter/Wallpaper.hpp"
#include "materials/matter/PaintedWall.hpp"
#include "materials/matter/Blackboard.hpp"
#include "materials/matter/Ceiling.hpp"
#include "materials/matter/Carpet.hpp"
//#include "materials/Fog.hpp"
#include "textures/Plain.hpp"
#include "textures/Tile.hpp"
#include "textures/ImageTexture.hpp"


class Level: public World {

public:

	virtual Spectrum infiniteSpectrum(Ray const& r) const override {
		Vector unit_direction = r.v.unit();
		float t = 0.5*(unit_direction.y + 1.0);
		return ((1.0-t)*Spectrum(1.0, 1.0, 1.0) + t*Spectrum(0.5, 0.7, 1.0));
		return Spectrum();
	}

	virtual Spectrum maxDepthSpectrum(Ray const& r) const override {
		return Spectrum();//0.171825*infiniteSpectrum(r);
	}

};

std::shared_ptr<World> getLevel1() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	std::vector<std::shared_ptr<Priority>> priorities = {
		std::make_shared<Priority>(Point(-1,0,3), 0.5, 0.5)
	};
	level->add(std::make_shared<Sphere>(Point(-1,0,3), 0.5, std::make_shared<Lamp>(std::make_shared<Plain>(25))));
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(std::make_shared<Plain>(0.5, 0.5, 0.5), priorities)));

	level->add(std::make_shared<Triangle>(Point(4,0,-2), Point(4,4,0), Point(4,0,2),
		std::make_shared<Lambertian>(std::make_shared<Tile>(Spectrum(1,1,1), Spectrum(0,1,0), 0.5), priorities)
	));
	level->add(std::make_shared<Sphere>(Point(-1,0,0), 0.5,
		std::make_shared<Dielectric>(1.5, std::make_shared<Plain>(0.01, 0.01, 0.01))
	));
	level->add(std::make_shared<Sphere>(Point(0,0,-1.5), 0.5,
		std::make_shared<Metal>(std::make_shared<Plain>(0.9), std::make_shared<Plain>(0))
	));
	level->add(std::make_shared<Sphere>(Point(0,0,1.5), 0.5, std::make_shared<BDF>(
		std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/earthmap.jpg"), Pi, Pi/2),
		std::make_shared<Plain>(getReflectance(1, 2)),
		std::make_shared<Plain>(0),
		std::make_shared<Plain>(0.4),
		nullptr,
		priorities
	), Orientation(Pi,0,0)));

	level->sort(false);
	//level->tree->print();

	return level;
}

std::shared_ptr<World> getLevel2() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	level->add(std::make_shared<Sphere>(Point(0,-100.5,-1), 100, std::make_shared<Lambertian>(std::make_shared<Plain>(0.5, 0.5, 0.5))));
	
	for (int a = -10; a < 10; a++) {
        for (int b = -10; b < 10; b++) {
            auto choose_mat = random_double();
            Point center(a, 0.2, b);

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    sphere_material = std::make_shared<Lambertian>(std::make_shared<Plain>(1,0,0));
                    level->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // metal
                    sphere_material = std::make_shared<Metal>(std::make_shared<Plain>(0,1,0), std::make_shared<Plain>(random_double(0, 0.5)));
                    level->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

	level->sort(false);
	//level->tree->print();
	return level;
}

std::shared_ptr<World> getLevel3() {
	std::shared_ptr<World> level = std::make_shared<Level>();
	std::vector<std::shared_ptr<Priority>> priorities = {
		//std::make_shared<Priority>(Point(700,1000,500), 4.36, 0.5)
		//std::make_shared<Priority>(Point(0,3,0), 0.1, 0.75)
		/*std::make_shared<Priority>(Point(6,1.5,-8), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-7), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-6), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-5), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-4), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-3), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-2), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,-1), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,0), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,1), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,2), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,3), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,4), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,5), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,6), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,7), 0.5, 0.03),
		std::make_shared<Priority>(Point(6,1.5,8), 0.5, 0.03)*/
	};

	std::shared_ptr<Brick> brick = std::make_shared<Brick>(1,1);
	std::shared_ptr<PaintedWall> painted = std::make_shared<PaintedWall>(1,1);
	std::shared_ptr<Wood> wood = std::make_shared<Wood>(Vector(1,0,0), Spectrum(0.925, 0.680, 0.373), 0.4);
	std::shared_ptr<Ceiling> ceiling = std::make_shared<Ceiling>(1,1);
	std::shared_ptr<Carpet> carpet = std::make_shared<Carpet>(1,1);

	level->add(std::make_shared<Rectangle>(Point(5,1,-10), Point(5,0,-10), Point(5,0,10), painted));
	level->add(std::make_shared<Rectangle>(Point(5,3,-10), Point(5,2,-10), Point(5,2,10), painted));
	level->add(std::make_shared<Rectangle>(Point(5,3,-10), Point(5,0,-10), Point(5,0,-9), painted));
	level->add(std::make_shared<Rectangle>(Point(5,3,9), Point(5,0,9), Point(5,0,10), painted));

	level->add(std::make_shared<Rectangle>(Point(-5,0,-10), Point(5,0,-10), Point(5,0,10), carpet));
	level->add(std::make_shared<Rectangle>(Point(-5,0,-10), Point(5,0,-10), Point(5,3,-10), painted));
	level->add(std::make_shared<Rectangle>(Point(-5,3,10), Point(5,3,10), Point(5,0,10), painted));
	level->add(std::make_shared<Rectangle>(Point(-5,0,-10), Point(-5,3,-10), Point(-5,3,10), painted));
	level->add(std::make_shared<Rectangle>(Point(-5,3,-10), Point(5,3,-10), Point(5,3,10), ceiling));

	level->add(std::make_shared<Rectangle>(Point(-3,1,-9.99), Point(3,1,-9.99), Point(3,2.5,-9.99), std::make_shared<Blackboard>(1,1,priorities)));

	level->add(std::make_shared<Parallelepiped>(Point(-2,0,-8), Point(2,0,-8), Point(2,1,-8), Point(2,1,-9), wood));
	level->add(std::make_shared<Sphere>(Point(-1,1.25,-8.5), 0.25, std::make_shared<Gold>(1, 1, priorities)));
	
	level->sort(false);
	//level->tree->print();
	return level;
}

int main() {
	auto level = getLevel3();

	Point pos = /*Point(-4, 1, 1);*/Point(-3.5,1.8,-4);
	Point dir = /*Point();*/Point(0,1,-10);
	Camera cam(level, pos, (dir - pos).unit(), Vector(0,1,0), 720, 16./9., 2.);

	std::time_t t0 = std::time(nullptr);

	Image image = cam.render(4, true, 2, 0.8, false, 100, 50);
	image.gaussian(3, 0.8);

	std::cout << nbr << " rays" << std::endl;
	std::cout << std::time(nullptr) - t0 << " seconds" << std::endl;
	
	std::cout << "average : " << image.getAverage().toBlackAndWhite() << std::endl;
	image.save("images/image.ppm");
	Image contour(image);
	contour.contour_detection();
	contour.save("images/image_contour.ppm");

	return 0;
}

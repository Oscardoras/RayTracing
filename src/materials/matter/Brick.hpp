#ifndef MATERIALS_MATTER_BRICK_H_
#define MATERIALS_MATTER_BRICK_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Brick: public BDF {

public:

	Brick(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/brick_wall_21/brick_wall_21_basecolor.png"), width, height),
			std::make_shared<Plain>(0/*getReflectance(1, 1.6)*/),
			std::make_shared<Plain>(0),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/brick_wall_21/brick_wall_21_roughness.png"), width, height),
			nullptr,//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/brick_wall_21/brick_wall_21_normal.png"), width, height),
			priorities,
			lightMap
		) {}

};


#endif

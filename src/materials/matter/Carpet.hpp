#ifndef MATERIALS_MATTER_CARPET_H_
#define MATERIALS_MATTER_CARPET_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Carpet: public BDF {

public:

	Carpet(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-carpet_floor_1/1K-carpet_floor_1-diffuse.jpg"), width, height),
			std::make_shared<Plain>(0),//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-carpet_floor_1/1K-carpet_floor_1-specular.jpg"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<Plain>(1),
			nullptr,//std::make_shared<ImageTexture>(std::make_shared<Image>("1K-carpet_floor_1-normal.jpg"), width, height),
			priorities,
			lightMap
		) {}

};


#endif

#ifndef MATERIALS_MATTER_BLACKBOARD_H_
#define MATERIALS_MATTER_BLACKBOARD_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Blackboard: public BDF {

public:

	Blackboard(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-blackboard/1K-blackboard-diffuse.jpg"), width, height),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-blackboard/1K-blackboard-specular.jpg"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<Plain>(0.3),
			nullptr,
			priorities,
			lightMap
		) {}

};


#endif

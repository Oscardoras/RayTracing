#ifndef MATERIALS_MATTER_CEILING_H_
#define MATERIALS_MATTER_CEILING_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Ceiling: public BDF {

public:

	Ceiling(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/false_ceiling_3/false_ceiling_basecolor.png"), width, height),
			std::make_shared<Plain>(0),//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/false_ceiling_3/false_ceiling_metallic.png"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/false_ceiling_3/false_ceiling_roughness.png"), width, height),
			nullptr,//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/false_ceiling_3/false_ceiling_normal.png"), width, height),
			priorities,
			lightMap
		) {}

};


#endif

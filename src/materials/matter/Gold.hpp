#ifndef MATERIALS_MATTER_GOLD_H_
#define MATERIALS_MATTER_GOLD_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Gold: public BDF {

public:

	Gold(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		BDF(
			std::make_shared<Plain>(0),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-Metallic_gold/1K-Metallic_gold_Base Color.jpg"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-Metallic_gold/1K-Metallic_gold_Roughness.jpg"), width, height),
			nullptr,
			priorities,
			lightMap
		) {}

};


#endif

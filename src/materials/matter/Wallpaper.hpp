#ifndef MATERIALS_MATTER_WALLPAPER_H_
#define MATERIALS_MATTER_WALLPAPER_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class Wallpaper: public BDF {

public:

	Wallpaper(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-wallpaper_42/1K-wallpaper_42_base_color.png"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<Plain>(0),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-wallpaper_42/1K-wallpaper_42_roughness.png"), width, height),
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-wallpaper_42/1K-wallpaper_42_normal.png"), width, height),
			priorities,
			lightMap
		) {}

};


#endif

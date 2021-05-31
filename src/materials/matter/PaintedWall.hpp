#ifndef MATERIALS_MATTER_PAINTEDWALL_H_
#define MATERIALS_MATTER_PAINTEDWALL_H_

#include "../BDF.hpp"
#include "../../textures/Plain.hpp"
#include "../../textures/ImageTexture.hpp"


class PaintedWall: public BDF {

public:

	PaintedWall(float width, float height, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> lightMap = nullptr):
		BDF(
			std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-painted_wall_3/1K-painted_wall_3-diffuse.jpg"), width, height),
			std::make_shared<Plain>(0),//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-painted_wall_3/1K-painted_wall_3-specular.jpg"), width, height),
			std::make_shared<Plain>(0),
			std::make_shared<Plain>(1),
			nullptr,//std::make_shared<ImageTexture>(std::make_shared<Image>("images/textures/1K-painted_wall_3/1K-painted_wall_3-normal.jpg"), width, height),
			priorities,
			lightMap
		) {}

};


#endif

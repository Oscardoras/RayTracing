#ifndef __MATERIALS__MODELS_LAMP_H__
#define __MATERIALS__MODELS_LAMP_H__

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


struct Lamp: public virtual Material {

	Texture* emitted;

	Lamp(Texture* const emitted);

	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

};


#endif

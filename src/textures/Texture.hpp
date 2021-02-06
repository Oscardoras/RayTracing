#ifndef TEXTURES_TEXTURE_H_
#define TEXTURES_TEXTURE_H_

#include "../algebra/Color.hpp"


class Texture {

public:

	virtual Color get(float const& u, float const& v) const = 0;

};


#endif

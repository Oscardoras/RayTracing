#ifndef __MATERIALS_MODELS_PHONG_H__
#define __MATERIALS_MODELS_PHONG_H__

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


struct LightSource {

	enum {
		PointSource,
		VectorSource
	} type;

	union {
		Point p;
		Vector v;
	};

	inline LightSource(Point const& p): type(PointSource), p(p) {}

	inline LightSource(Vector const& v): type(VectorSource), v(v) {}

	inline Vector getDirection(Point const& from) const {
		if (type == PointSource) return (p - from).unit();
		else return v;
	}

};


class Phong: public Material {

protected:

	Texture* ambiant;
	Texture* diffuse;
	Texture* specular;
	Texture* alpha;
	std::vector<LightSource> light_sources;

public:

	Phong(Texture* const ambiant, Texture* const diffuse, Texture* const specular, Texture* const alpha, std::vector<LightSource> const& light_sources);
	
	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

};


#endif

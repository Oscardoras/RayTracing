#include "Tile.hpp"

#include "../MathUtils.hpp"


Tile::Tile(Spectrum const& spectrum1, Spectrum const& spectrum2, float const size): spectrum1(spectrum1), spectrum2(spectrum2), size(size) {}

Spectrum Tile::getSpectrum(RelativePosition const& relative) const {
	if (std::isnan(relative.u) || std::isnan(relative.v)) return Spectrum();
	int a = int(relative.u / size);
	int b = int(relative.v / size);
	return a%2 == b%2 ? spectrum1 : spectrum2;
}

Vector Tile::getVector(RelativePosition const& relative) const {
	return getSpectrum(relative).toVector();
}

float Tile::getFloat(RelativePosition const& relative) const {
	return getSpectrum(relative).getIntensity();
}
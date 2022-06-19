#include "Tile.hpp"


Tile::Tile(Spectrum const& spectrum1, Spectrum const& spectrum2, float const size): spectrum1(spectrum1), spectrum2(spectrum2), size(size) {}

Spectrum Tile::getSpectrum(RelativePosition const& relative) const {
	if (std::isnan(relative.u) || std::isnan(relative.v)) return Spectrum();
	int a = int(std::floor(relative.u / size));
	int b = int(std::floor(relative.v / size));
	return modulo(a, 2) == modulo(b, 2) ? spectrum1 : spectrum2;
}

Vector Tile::getVector(RelativePosition const& relative) const {
	return getSpectrum(relative).toVector();
}

float Tile::getFloat(RelativePosition const& relative) const {
	return getSpectrum(relative).getIntensity();
}
#include "Lamp.hpp"


Lamp::Lamp(Texture* const emitted): emitted(emitted) {}

Light Lamp::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	return Light(emitted->getSpectrum(position));
}
#include "Plain.hpp"


Plain::Plain(Spectrum const& spectrum): spectrum(spectrum) {}

Plain::Plain(float const r, float const g, float const b): spectrum(Color(r, g, b).toSpectrum()) {}

Plain::Plain(float const f): spectrum(Color(f,f,f).toSpectrum()) {}

Spectrum Plain::getSpectrum(RelativePosition const& relative) const {
	return spectrum;
}

Vector Plain::getVector(RelativePosition const& relative) const {
	return spectrum.toColor().toVector();
}

float Plain::getFloat(RelativePosition const& relative) const {
	return spectrum.getIntensity();
}
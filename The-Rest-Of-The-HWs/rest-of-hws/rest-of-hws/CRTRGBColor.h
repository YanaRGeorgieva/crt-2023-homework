#ifndef CRTRGBCOLOR_H
#define CRTRGBCOLOR_H

#include "CRTVector.h"

const int maxRGBColorComponent = 255;
struct CRTRGBColor {
	unsigned char r, g, b;

	CRTRGBColor(const int r = 0, const int g = 0, const int b = 0) :
		r(r% maxRGBColorComponent),
		g(g% maxRGBColorComponent),
		b(b% maxRGBColorComponent) { }

	CRTRGBColor(const CRTVector& rhs) {
		r = (unsigned char)(rhs.x * maxRGBColorComponent);
		g = (unsigned char)(rhs.y * maxRGBColorComponent);
		b = (unsigned char)(rhs.z * maxRGBColorComponent);
		//*this = toRGB32(rhs);
	}

	const CRTRGBColor& operator=(const CRTVector& rhs) {
		r = (unsigned char)(rhs.x * maxRGBColorComponent);
		g = (unsigned char)(rhs.y * maxRGBColorComponent);
		b = (unsigned char)(rhs.z * maxRGBColorComponent);
		//*this = toRGB32(rhs);
		return *this;
	}

	int fastFloor(float x) const { return int(x) - (x<int(x)); }

	float clamp(float x, float a, float b) const {
		return std::fminf(std::fmaxf(x, a), b);
	}

	CRTRGBColor toRGB32(const CRTVector& rhs) const {
		return CRTRGBColor(
			fastFloor(clamp(rhs.x * 256.0f, 0.0f, 255.0f)),
			fastFloor(clamp(rhs.y * 256.0f, 0.0f, 255.0f)),
			fastFloor(clamp(rhs.z * 256.0f, 0.0f, 255.0f)));
	}
};

#endif // CRTRGBCOLOR_H
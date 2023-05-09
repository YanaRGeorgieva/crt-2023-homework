#ifndef CRTRGBCOLOR_H
#define CRTRGBCOLOR_H

#include "CRTVector.h"

const int maxRGBColorComponent = 255;
struct CRTRGBColor {
	unsigned char r, g, b;

	CRTRGBColor(const int r = 0, const int g = 0, const int b = 0) :
		r(r% maxRGBColorComponent),
		g(g% maxRGBColorComponent),
		b(b% maxRGBColorComponent) {}

	CRTRGBColor(const CRTVector& rhs) {
		*this = toRGB32(rhs);
	}

	const CRTRGBColor& operator=(const CRTVector& rhs) {
		*this = toRGB32(rhs);
		return *this;
	}

	CRTRGBColor toRGB32(const CRTVector& rhs) {
		CRTRGBColor ret;
		ret.r = (rhs.x > 1.0f || equals(rhs.x, 1.0f)) ? maxRGBColorComponent : (unsigned char)(rhs.x * 256.0f);
		ret.g = (rhs.y > 1.0f || equals(rhs.y, 1.0f)) ? maxRGBColorComponent : (unsigned char)(rhs.y * 256.0f);
		ret.b = (rhs.z > 1.0f || equals(rhs.z, 1.0f)) ? maxRGBColorComponent : (unsigned char)(rhs.z * 256.0f);
		return ret;
	}
};

#endif // CRTRGBCOLOR_H
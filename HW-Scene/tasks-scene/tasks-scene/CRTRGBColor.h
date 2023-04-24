#ifndef CRTRGBCOLOR_H
#define CRTRGBCOLOR_H

#include "utils.h"
#include "CRTVector.h"

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
	}

	const CRTRGBColor& operator=(const CRTVector& rhs) {
		r = (unsigned char)(rhs.x * maxRGBColorComponent);
		g = (unsigned char)(rhs.y * maxRGBColorComponent);
		b = (unsigned char)(rhs.z * maxRGBColorComponent);
		return *this;
	}
};

#endif // CRTRGBCOLOR_H
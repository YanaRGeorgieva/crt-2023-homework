#ifndef CRTLIGHT_H
#define CRTLIGHT_H

#include "CRTVector.h"

struct CRTLight {
	CRTVector position;
	float intensity;
	CRTLight(const CRTVector& position, const float intensity) : position(position), intensity(intensity) { };
};

#endif // !CRTLIGHT_H
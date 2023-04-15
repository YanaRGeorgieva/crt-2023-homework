#ifndef CRTRAY_H
#define CRTRAY_H
#include "CRTVector.h"

struct CRTRay {
	CRTVector origin;
	CRTVector direction;
	CRTRay(const CRTVector& origin, const CRTVector& direction) : origin(origin), direction(direction) { };
};

#endif // !CRTRAY_H
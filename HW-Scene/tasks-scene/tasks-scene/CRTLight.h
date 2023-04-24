#ifndef CRTLIGHT_H
#define CRTLIGHT_H

#include "CRTVector.h"

struct CRTLight {
	CRTVector position;
	CRTLight(const CRTVector& position) : position(position) { };
};

#endif // !CRTLIGHT_H
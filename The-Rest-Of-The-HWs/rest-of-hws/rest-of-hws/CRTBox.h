#ifndef CRTBOX_H
#define CRTBOX_H

#include <vector>

#include "constants.h"
#include "CRTVector.h"
#include "CRTTriangle.h"

struct retIntersectionBox {
	bool isValid = false;
	float t = 0.0f;
	CRTVector p = zeroVector;
};

class CRTBox {
public:
	CRTBox(const CRTVector& boxMin = CRTVector(MAX_FLOAT),
		const CRTVector& boxMax = CRTVector(MIN_FLOAT)) :
		boxMin(boxMin), boxMax(boxMax) {}

	bool isValid() const;
	void expand(const CRTVector& vertex);
	retIntersectionBox intersect(const CRTRay& ray, const float& thresholdT) const;
private:
	CRTVector boxMin, boxMax;
};

#endif // !CRTBOX_H

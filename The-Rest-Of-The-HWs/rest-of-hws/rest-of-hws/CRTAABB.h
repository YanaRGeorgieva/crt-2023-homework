#ifndef CRTAABB_H
#define CRTAABB_H

#include <vector>

#include "constants.h"
#include "CRTVector.h"
#include "CRTTriangle.h"

struct retIntersectionBox {
	bool isValid;
	float t;
	CRTVector p;
};

class CRTAABB {
public:
	CRTAABB(const CRTVector& boxMin = CRTVector(maxFloat),
		const CRTVector& boxMax = CRTVector(minFloat)) :
		boxMin(boxMin), boxMax(boxMax) {}

	bool isValid() const;
	void expand(const CRTVector& vertex);
	retIntersectionBox intersect(const CRTRay& ray, const float& thresholdT) const;
private:
	CRTVector boxMin, boxMax;
};

#endif // !CRTAABB_H

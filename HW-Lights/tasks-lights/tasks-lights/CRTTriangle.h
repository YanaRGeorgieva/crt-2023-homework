#ifndef CRTTRIANGLE_H
#define CRTTRIANGLE_H

#include <cmath>
#include <vector>

#include "utils.h"
#include "CRTRay.h"
#include "CRTVector.h"

const int numberOFVertices = 3;
struct CRTTriangle {
	CRTVector v0, v1, v2;
	CRTVector triNormalVector;
	CRTVector e0, e1, e2;

	CRTTriangle(const CRTVector& x, const CRTVector& y, const CRTVector& z) :v0(x), v1(y), v2(z) {
		// Generate triangles normal vectors : normalize(cross(E0, E1)) - precomputed 
		triNormalVector = getNormalVector();
		e0 = v1 - v0;
		e1 = v2 - v1;
		e2 = v0 - v2;
	}

	CRTVector& operator [](const int idx) {
		return (&v0)[idx];
	}

	const CRTVector& operator [](const int idx) const {
		return (&v0)[idx];
	}

	CRTVector getNormalVector() const;

	float getArea() const;

	CRTVector intersect(const CRTRay& ray) const;
};

#endif // !CRTTRIANGLE_H

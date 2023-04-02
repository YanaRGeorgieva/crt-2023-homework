#ifndef CRTTRIANGLE_H
#define CRTTRIANGLE_H
#include "CRTVector.h"
#include <cmath>

const int numberOFVertices = 3;
struct CRTTriangle {
	CRTVector v0, v1, v2;

	CRTTriangle(const CRTVector& x, const CRTVector& y, const CRTVector& z) :v0(x), v1(y), v2(z) {}

	CRTVector getNormalVector() const {
		CRTVector e0 = v1 - v0;
		CRTVector e1 = v2 - v0;
		return e0.cross(e1).normalize();
	}

	float getArea() const {
		CRTVector e0 = v1 - v0;
		CRTVector e1 = v2 - v0;
		return e0.cross(e1).length() / 2;
	}

	CRTVector& operator [](const int idx) {
		return (&v0)[idx];
	}

	const CRTVector& operator [](const int idx) const {
		return (&v0)[idx];
	}
};

struct NormalsAndSides {
	CRTVector triNormalVector;
	CRTVector e0;
	CRTVector e1;
	CRTVector e2;

	NormalsAndSides(const CRTTriangle& tri) : triNormalVector(tri.getNormalVector()), e0(tri.v1 - tri.v0), e1(tri.v2 - tri.v1), e2(tri.v0 - tri.v2) {}
};


#endif // !CRTTRIANGLE_H

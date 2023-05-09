#ifndef CRTTRIANGLE_H
#define CRTTRIANGLE_H

#include <cmath>
#include <vector>

#include "CRTRay.h"
#include "constants.h"
#include "CRTVector.h"

const int numberOFVertices = 3;
class CRTTriangle {
public:
	CRTTriangle(const CRTVector& x, const CRTVector& y, const CRTVector& z) :v0(x), v1(y), v2(z) {
		// Generate triangles normal vectors : normalize(cross(E0, E1)) - precomputed 
		computeFaceNormal();
		e0 = v1 - v0;
		e1 = v2 - v1;
		e2 = v0 - v2;
	}

	const CRTVector& operator [](const int idx) const {
		return (&v0)[idx];
	}

	CRTVector getFaceNormal() const;

	float getArea() const;

	struct retDataFromTriIntersect {
		CRTVector p;
		CRTVector barycentricCoordinates;
		float t;
	};

	retDataFromTriIntersect intersect(const CRTRay& ray, const float thresholdT = std::numeric_limits<float>::max()) const;
private:
	void computeFaceNormal();

	CRTVector v0, v1, v2;
	CRTVector faceNormal;
	CRTVector e0, e1, e2;
};

#endif // !CRTTRIANGLE_H

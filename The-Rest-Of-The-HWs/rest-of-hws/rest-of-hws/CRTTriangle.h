#ifndef CRTTRIANGLE_H
#define CRTTRIANGLE_H

#include <cmath>
#include <vector>

#include "CRTRay.h"
#include "constants.h"
#include "CRTVector.h"


class CRTTriangle {
public:
	CRTTriangle(const CRTVector& x, const CRTVector& y, const CRTVector& z, const CRTUintVector& facesIndices) :v0(x), v1(y), v2(z), verticesFacesIndices(facesIndices) {
		// Generate triangles normal vectors : normalize(cross(E0, E1)) - precomputed 
		computeFaceNormal();
		e0 = v1 - v0;
		e1 = v2 - v1;
		e2 = v0 - v2;
		centroid = (v0 + v1 + v2) / 3.0f;
	}

	const CRTVector& operator [](const int idx) const {
		return (&v0)[idx];
	}

	CRTVector getFaceNormal() const;

	float getArea() const;

	const CRTUintVector& getVerticesFacesIndices() const;

	const CRTVector& getCentroid() const;

	struct retDataFromTriangleIntersect {
		CRTVector p;
		CRTVector barycentricCoordinates;
		float t;
		bool isValid;
	};

	retDataFromTriangleIntersect intersect(const CRTRay& ray, const float thresholdT = 1e30f) const;
private:
	void computeFaceNormal();

	CRTVector v0, v1, v2;
	CRTUintVector verticesFacesIndices;
	CRTVector centroid;
	CRTVector faceNormal;
	CRTVector e0, e1, e2;
};

#endif // !CRTTRIANGLE_H

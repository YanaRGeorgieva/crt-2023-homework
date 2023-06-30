#include "CRTTriangle.h"

void CRTTriangle::computeFaceNormal() {
	CRTVector e0 = v1 - v0;
	CRTVector e1 = v2 - v0;
	faceNormal = e0.cross(e1).normalize();
}

CRTVector CRTTriangle::getFaceNormal() const {
	CRTVector e0 = v1 - v0;
	CRTVector e1 = v2 - v0;
	return e0.cross(e1).normalize();
}

float CRTTriangle::getArea() const {
	CRTVector e0 = v1 - v0;
	CRTVector e1 = v2 - v0;
	return e0.cross(e1).length() / 2.0f;
}

CRTTriangle::retDataFromTriangleIntersect CRTTriangle::intersect(const CRTRay& ray, const float thresholdT) const {
	retDataFromTriangleIntersect worstResult{};
	worstResult.isValid = false;

	// If R is not parallel to the triangle’s plane : dot(N, R) != 0
	const float rProj = faceNormal.dot(ray.direction);
	if (equals(rProj, 0.0f)) {
		return worstResult;
	}

	// Calculate distance to triangle's plane
	const float rpDist = (v0 - ray.origin).dot(faceNormal);

	// Find R - plane intersection point P : t = rpDist / rProj; p = t * rDir
	const float t = rpDist / rProj;
	const CRTVector p = ray.origin + t * ray.direction;

	// For shading t > 0
	if (!greaterEqThan(t, 0.0f) || lessThan(thresholdT, t)) {
		return worstResult;
	}

	// Check if P is on the left of E0 : dot(N, cross(E0, V0P)) > 0
	const CRTVector v0p = p - v0;
	const float dotTriNormalWithe0xv0p = faceNormal.dot(e0.cross(v0p));
	if (!greaterEqThan(dotTriNormalWithe0xv0p, 0.0f)) {
		return worstResult;
	}

	// Check if P is on the left of E1 : dot(N, cross(E1, V1P)) > 0
	const CRTVector v1p = p - v1;
	const float dotTriNormalWithe1xv1p = faceNormal.dot(e1.cross(v1p));
	if (!greaterEqThan(dotTriNormalWithe1xv1p, 0.0f)) {
		return worstResult;
	}

	// Check if P is on the left of E2 : dot(N, cross(E2, V2P)) > 0
	const CRTVector v2p = p - v2;
	const float dotTriNormalWithe2xv2p = faceNormal.dot(e2.cross(v2p));
	if (!greaterEqThan(dotTriNormalWithe2xv2p, 0.0f)) {
		return worstResult;
	}

	const CRTVector v0v2 = v2 - v0;
	const CRTVector v0v1 = v1 - v0;
	const float wholeAreaTimes2 = v0v1.cross(v0v2).length();
	const float u = v0p.cross(v0v2).length() / wholeAreaTimes2;
	const float v = v0v1.cross(v0p).length() / wholeAreaTimes2;
	const float w = 1.0f - u - v;
	CRTVector barycentricCoordinates(u, v, w);

	return retDataFromTriangleIntersect{ p, barycentricCoordinates, t, true };
}

const CRTUintVector& CRTTriangle::getVerticesFacesIndices() const {
	return verticesFacesIndices;
}

const CRTVector& CRTTriangle::getCentroid() const {
	return centroid;
}
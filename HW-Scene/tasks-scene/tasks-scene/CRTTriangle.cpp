#include "CRTTriangle.h"

CRTVector CRTTriangle::getNormalVector() const {
	CRTVector e0 = v1 - v0;
	CRTVector e1 = v2 - v0;
	return e0.cross(e1).normalize();
}

float CRTTriangle::getArea() const {
	CRTVector e0 = v1 - v0;
	CRTVector e1 = v2 - v0;
	return e0.cross(e1).length() / 2;
}

CRTVector CRTTriangle::intersect(const CRTRay& cameraRay) const {
	// If R is not parallel to the triangle’s plane : dot(N, R) != 0
	if (equals(cameraRay.direction.dot(triNormalVector), 0.0f)) {
		return worstP;
	}

	// If R is towards the triangle’s plane : dot(V0, N) < 0
	const float rpDist = (v0 - cameraRay.origin).dot(triNormalVector);
	if (rpDist > 0.0f || equals(rpDist, 0.0f)) {
		return worstP;
	}

	// Find R - plane intersection point P : t = rpDist / rProj; p = t * rDir
	const float rProj = triNormalVector.dot(cameraRay.direction);
	const float t = rpDist / rProj;
	const CRTVector p = cameraRay.origin + t * cameraRay.direction;

	//	Check if P is on the left of E0 : dot(N, cross(E0, V0P)) > 0
	const CRTVector v0p = p - v0;
	const float dotTriNormalWithe0xv0p = triNormalVector.dot(e0.cross(v0p));
	if (dotTriNormalWithe0xv0p < 0.0f || equals(dotTriNormalWithe0xv0p, 0.0f)) {
		return worstP;
	}

	//	Check if P is on the left of E1 : dot(N, cross(E1, V1P)) > 0
	const CRTVector v1p = p - v1;
	const float dotTriNormalWithe1xv1p = triNormalVector.dot(e1.cross(v1p));
	if (dotTriNormalWithe1xv1p < 0.0f || equals(dotTriNormalWithe1xv1p, 0.0f)) {
		return worstP;
	}

	//	Check if P is on the left of E2 : dot(N, cross(E2, V2P)) > 0
	const CRTVector v2p = p - v2;
	const float dotTriNormalWithe2xv2p = triNormalVector.dot(e2.cross(v2p));
	if (dotTriNormalWithe2xv2p < 0.0f || equals(dotTriNormalWithe2xv2p, 0.0f)) {
		return worstP;
	}

	return p;
}
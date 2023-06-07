#ifndef CRTRAY_H
#define CRTRAY_H

#include "CRTVector.h"

enum class RayType {
	invaid,
	camera, ///< Ray shot from camera
	shadow, ///< When shading diffuse material
	reflective, ///< Aftre reflective material is hit
	refractive ///< After refractive material is hit
};

///  Ray for tracing in the scene
struct CRTRay {
	CRTVector origin; ///< Where the ray is shot from
	CRTVector direction; ///< Normalized vector representing the direction of the ray
	int pathDepth; ///< How many rays led to this one on the light path
	RayType type; ///< The purpose of the ray
	CRTRay(const CRTVector& origin, const CRTVector& direction, const int pathDepth = 0, const RayType& type = RayType::camera) :
		origin(origin), direction(direction), pathDepth(pathDepth), type(type) {};
};

#endif // !CRTRAY_H
#ifndef CRTMATERIAL_H
#define CRTMATERIAL_H

#include <string>

#include "CRTVector.h"

struct CRTMaterial {
	CRTVector albedo;
	std::string type;
	bool isSmoothShaded;

	CRTMaterial(const CRTVector& albedo = CRTVector(1.0f),
				const std::string& type = "diffuse",
				const bool isSmoothShaded = false) :
		albedo(albedo), type(type), isSmoothShaded(isSmoothShaded) { };
};

#endif // !CRTMATERIAL_H
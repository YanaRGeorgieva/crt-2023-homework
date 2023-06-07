#ifndef CRTMATERIAL_H
#define CRTMATERIAL_H

#include <string>

#include "CRTVector.h"

struct CRTMaterial {
	enum MaterialTypes {
		diffuse = 0,
		reflective,
		refractive,
		constant,
		undefined
	};

	CRTVector albedo;
	float ior;
	MaterialTypes type;
	bool isSmoothShaded;

	CRTMaterial(const CRTVector& albedo = CRTVector(1.0f),
		const std::string& type = "diffuse",
		const bool isSmoothShaded = false,
		const float& ior = 0.0f) :
		albedo(albedo), isSmoothShaded(isSmoothShaded), ior(ior) {
		if (type == "diffuse") {
			this->type = diffuse;
		} else if (type == "reflective") {
			this->type = reflective;
		} else if (type == "refractive") {
			this->type = refractive;
		} else if (type.empty()) {
			this->type = constant;
		} else {
			this->type = undefined;
		}
	};
};

#endif // !CRTMATERIAL_H
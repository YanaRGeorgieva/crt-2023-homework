#ifndef CRTMATERIAL_H
#define CRTMATERIAL_H

#include <string>

#include "CRTVector.h"

struct CRTMaterial {
	enum MaterialTypes {
		DIFFUSE = 0,
		REFLECTIVE,
		UNDEFINED
	};

	CRTVector albedo;
	MaterialTypes type;
	bool isSmoothShaded;

	CRTMaterial(const CRTVector& albedo = CRTVector(1.0f),
		const std::string& type = "diffuse",
		const bool isSmoothShaded = false) :
		albedo(albedo), isSmoothShaded(isSmoothShaded) {
		if (type == "diffuse") {
			this->type = DIFFUSE;
		} else if (type == "reflective") {
			this->type = REFLECTIVE;
		} else {
			this->type = UNDEFINED;
		}
	};
};

#endif // !CRTMATERIAL_H
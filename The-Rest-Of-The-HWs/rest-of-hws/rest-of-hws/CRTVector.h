#ifndef CRTVECTOR_H
#define CRTVECTOR_H

#include <cmath>

#include "constants.h"

struct CRTVector {
	float x, y, z;

	CRTVector() : x(0.0f), y(0.0f), z(0.0f) { }

	CRTVector(const float num) : x(num), y(num), z(num) { }

	CRTVector(const float x, const float y, const float z) : x(x), y(y), z(z) { }

	float& operator [](const int idx) {
		return (&x)[idx % 3];
	}

	const float& operator [](const int idx) const {
		return (&x)[idx % 3];
	}

	float length() const;

	CRTVector normalize() const;

	void setComponents(const float x, const float y, const float z);

	CRTVector operator+ (const CRTVector& rhs) const;
	void operator+= (const CRTVector& rhs);

	bool operator== (const CRTVector& rhs) const;
	bool operator!= (const CRTVector& rhs) const;

	CRTVector operator- (const CRTVector& rhs) const;

	CRTVector operator* (const CRTVector& rhs) const;

	CRTVector operator* (const int scalar) const;
	CRTVector operator* (const float scalar) const;
	CRTVector operator* (const double scalar) const;

	CRTVector cross(const CRTVector& rhs) const;

	float dot(const CRTVector& rhs) const;

	CRTVector operator/ (const int scalar) const;
};

const CRTVector worstP(MAX_FLOAT);

const CRTVector zeroVector(0.0f);
const CRTVector identityVector(1.0f);

CRTVector operator* (const int scalar, const CRTVector& rhs);
CRTVector operator* (const float scalar, const CRTVector& rhs);
CRTVector operator* (const double scalar, const CRTVector& rhs);

#endif // !CRTVECTOR_H

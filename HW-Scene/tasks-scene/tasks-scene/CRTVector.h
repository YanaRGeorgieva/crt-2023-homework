#ifndef CRTVECTOR_H
#define CRTVECTOR_H

#include <limits>
#include <cmath>

struct CRTVector {
	float x, y, z;

	CRTVector() : x(), y(), z() { }

	CRTVector(float x, float y, float z) : x(x), y(y), z(z) { }

	float& operator [](const int idx) {
		return (&x)[idx];
	}

	const float& operator [](const int idx) const {
		return (&x)[idx];
	}

	float length() const;

	CRTVector normalize() const;

	void setComponents(float x, float y, float z);

	CRTVector operator+ (const CRTVector& rhs) const;
	void operator+= (const CRTVector& rhs);

	CRTVector operator- (const CRTVector& rhs) const;

	CRTVector operator* (const CRTVector& rhs) const;

	CRTVector operator* (const int scalar) const;
	CRTVector operator* (const float scalar) const;
	CRTVector operator* (const double scalar) const;

	CRTVector cross(const CRTVector& rhs) const;

	float dot(const CRTVector& rhs) const;

	CRTVector operator/ (const int scalar) const;
};

const CRTVector worstP{
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max() ,
		std::numeric_limits<float>::max() };

const CRTVector zeroVector{ 0.0f, 0.0f, 0.0f };
const CRTVector identityVector{ 1.0f, 1.0f, 1.0f };

CRTVector operator* (const int scalar, const CRTVector& rhs);
CRTVector operator* (const float scalar, const CRTVector& rhs);
CRTVector operator* (const double scalar, const CRTVector& rhs);

#endif // !CRTVECTOR_H

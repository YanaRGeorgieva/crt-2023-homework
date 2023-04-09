#ifndef CRTVECTOR_H
#define CRTVECTOR_H

#include <limits>
#include <cmath>

struct CRTVector {
	float x, y, z;

	static const CRTVector worstP;
	static const CRTVector zeroVector;
	static const CRTVector identityVector;

	CRTVector() : x(), y(), z() { }

	CRTVector(float x, float y, float z) : x(x), y(y), z(z) { }

	float& operator [](const int idx) {
		return (&x)[idx];
	}

	const float& operator [](const int idx) const {
		return (&x)[idx];
	}

	float length() const {
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	CRTVector normalize() const {
		float multiplier = 1.0f / this->length();
		return CRTVector{ x * multiplier, y * multiplier, z * multiplier };
	}

	void setComponents(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CRTVector operator+ (const CRTVector& rhs) const {
		return CRTVector{ x + rhs.x, y + rhs.y, z + rhs.z };
	}

	CRTVector operator- (const CRTVector& rhs) const {
		return CRTVector{ x - rhs.x, y - rhs.y, z - rhs.z };
	}

	CRTVector operator* (const CRTVector& rhs) const {
		return CRTVector{ x * rhs.x, y * rhs.y, z * rhs.z };
	}

	CRTVector operator* (const int scalar) const {
		return CRTVector{ x * scalar, y * scalar, z * scalar };
	}

	CRTVector operator* (const float scalar) const {
		return CRTVector{ x * scalar, y * scalar, z * scalar };
	}

	CRTVector operator* (const double scalar) const {
		return CRTVector{ x * (float)scalar, y * (float)scalar, z * (float)scalar };
	}

	CRTVector cross(const CRTVector& rhs) const {
		return CRTVector{
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		};
	}

	float dot(const CRTVector& rhs) const {
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}

};

inline CRTVector operator* (const int scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * scalar, rhs.y * scalar, rhs.z * scalar };
}

inline CRTVector operator* (const float scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * scalar, rhs.y * scalar, rhs.z * scalar };
}

inline CRTVector operator* (const double scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * (float)scalar, rhs.y * (float)scalar, rhs.z * (float)scalar };
}

static const CRTVector worstP{
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max() ,
		std::numeric_limits<float>::max() };

static const CRTVector zeroVector{ 0.0f, 0.0f, 0.0f };
static const CRTVector identityVector{ 1.0f, 1.0f, 1.0f };


#endif // !CRTVECTOR_H

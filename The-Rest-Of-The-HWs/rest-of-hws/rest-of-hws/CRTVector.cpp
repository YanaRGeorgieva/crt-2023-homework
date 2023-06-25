#include "CRTVector.h"

float CRTVector::length() const {
	return sqrtf((x * x) + (y * y) + (z * z));
}

CRTVector CRTVector::normalize() const {
	const float multiplier = 1.0f / this->length();
	return CRTVector{ x * multiplier, y * multiplier, z * multiplier };
}

void CRTVector::setComponents(const float x, const float y, const float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

CRTVector CRTVector::operator+ (const CRTVector& rhs) const {
	return CRTVector{ x + rhs.x, y + rhs.y, z + rhs.z };
}

bool CRTVector::operator== (const CRTVector& rhs) const {
	return equals(x, rhs.x) && equals(y, rhs.y) && equals(z, rhs.z);
}

bool CRTVector::operator!= (const CRTVector& rhs) const {
	return !(*this == rhs);
}

void CRTVector::operator+= (const CRTVector& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

CRTVector CRTVector::operator- (const CRTVector& rhs) const {
	return CRTVector{ x - rhs.x, y - rhs.y, z - rhs.z };
}

CRTVector CRTVector::operator* (const CRTVector& rhs) const {
	return CRTVector{ x * rhs.x, y * rhs.y, z * rhs.z };
}

CRTVector CRTVector::operator* (const int scalar) const {
	return CRTVector{ x * (float)scalar, y * (float)scalar, z * (float)scalar };
}

CRTVector CRTVector::operator* (const float scalar) const {
	return CRTVector{ x * scalar, y * scalar, z * scalar };
}

CRTVector CRTVector::operator* (const double scalar) const {
	return CRTVector{ x * (float)scalar, y * (float)scalar, z * (float)scalar };
}

CRTVector CRTVector::cross(const CRTVector& rhs) const {
	return CRTVector{
		y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x
	};
}

float CRTVector::dot(const CRTVector& rhs) const {
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

CRTVector CRTVector::operator/ (const int scalar) const {
	return CRTVector{ x / (float)scalar, y / (float)scalar, z / (float)scalar };
}

CRTVector operator* (const int scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * (float)scalar, rhs.y * (float)scalar, rhs.z * (float)scalar };
}

CRTVector operator* (const float scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * scalar, rhs.y * scalar, rhs.z * scalar };
}

CRTVector  operator* (const double scalar, const CRTVector& rhs) {
	return CRTVector{ rhs.x * (float)scalar, rhs.y * (float)scalar, rhs.z * (float)scalar };
}
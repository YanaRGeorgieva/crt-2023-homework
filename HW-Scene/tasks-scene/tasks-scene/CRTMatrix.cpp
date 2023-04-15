#include "CRTMatrix.h"

void CRTMatrix::set(const CRTMatrix& rhs) {
	v0 = rhs.v0;
	v1 = rhs.v1;
	v2 = rhs.v2;
}

void CRTMatrix::setCol(const int i, const CRTVector& a) {
	(*this)[0][i] = a.x, (*this)[1][i] = a.y, (*this)[2][i] = a.z;
}

CRTVector CRTMatrix::getCol(const int i) const {
	return CRTVector((*this)[0][i], (*this)[1][i], (*this)[2][i]);
}

CRTMatrix CRTMatrix::CRTMatrix::transpose() const {
	return CRTMatrix(getCol(0), getCol(1), getCol(2));

}

CRTMatrix CRTMatrix::negate() const {
	return CRTMatrix(-1 * v0, -1 * v1, -1 * v2);

}

CRTMatrix CRTMatrix::operator +(const CRTMatrix& rhs) const {
	return CRTMatrix(v0 + rhs.v0, v1 + rhs.v1, v2 + rhs.v2);
}

CRTMatrix CRTMatrix::operator -(const CRTMatrix& rhs) const {
	return CRTMatrix(v0 - rhs.v0, v1 - rhs.v1, v2 - rhs.v2);
}

CRTMatrix CRTMatrix::operator *(const CRTMatrix& rhs) const {
	return CRTMatrix(
		CRTVector(
			v0[0] * rhs.v0[0] + v0[1] * rhs.v1[0] + v0[2] * rhs.v2[0],
			v0[0] * rhs.v0[1] + v0[1] * rhs.v1[1] + v0[2] * rhs.v2[1],
			v0[0] * rhs.v0[2] + v0[1] * rhs.v1[2] + v0[2] * rhs.v2[2]),
		CRTVector(
			v1[0] * rhs.v0[0] + v1[1] * rhs.v1[0] + v1[2] * rhs.v2[0],
			v1[0] * rhs.v0[1] + v1[1] * rhs.v1[1] + v1[2] * rhs.v2[1],
			v1[0] * rhs.v0[2] + v1[1] * rhs.v1[2] + v1[2] * rhs.v2[2]),
		CRTVector(
			v2[0] * rhs.v0[0] + v2[1] * rhs.v1[0] + v2[2] * rhs.v2[0],
			v2[0] * rhs.v0[1] + v2[1] * rhs.v1[1] + v2[2] * rhs.v2[1],
			v2[0] * rhs.v0[2] + v2[1] * rhs.v1[2] + v2[2] * rhs.v2[2])
	);
}


CRTVector operator *(const CRTVector& a, const CRTMatrix& m) {
	return CRTVector(a.dot(m.getCol(0)), a.dot(m.getCol(1)), a.dot(m.getCol(2)));
}

//CRTVector operator *(const CRTMatrix& m, const CRTVector& a) {
//	return a.x * m.v0 + a.y * m.v1 + a.z * m.v2;
//}

CRTMatrix operator *(const CRTMatrix& m, const int x) {
	return CRTMatrix(m.v0 * x, m.v1 * x, m.v2 * x);
}

CRTMatrix operator *(const int x, const CRTMatrix& m) {
	return CRTMatrix(x * m.v0, x * m.v1, x * m.v2);
}

CRTMatrix operator *(const CRTMatrix& m, const float x) {
	return CRTMatrix(m.v0 * x, m.v1 * x, m.v2 * x);
}

CRTMatrix operator *(const float x, const CRTMatrix& m) {
	return CRTMatrix(x * m.v0, x * m.v1, x * m.v2);
}

CRTMatrix operator *(const CRTMatrix& m, const double& x) {
	return CRTMatrix(m.v0 * x, m.v1 * x, m.v2 * x);
}

CRTMatrix operator *(const double& x, const CRTMatrix& m) {
	return CRTMatrix(x * m.v0, x * m.v1, x * m.v2);
}

CRTMatrix makeRotationCRTMatrixX(const float xangle) {
	const float xrot = (float)xangle * ((float)M_PI / 180.0f);
	const float s = sin(xrot);
	const float c = cos(xrot);
	return CRTMatrix(CRTVector(1.0f, 0.0f, 0.0f), CRTVector(0, c, s), CRTVector(0, -s, c));
}

CRTMatrix makeRotationCRTMatrixY(const float yangle) {
	const float yrot = (float)yangle * ((float)M_PI / 180.0f);
	const float s = sin(yrot);
	const float c = cos(yrot);
	return CRTMatrix(CRTVector(c, 0, -s), CRTVector(0.0f, 1.0f, 0.0f), CRTVector(s, 0, c));
}

CRTMatrix makeRotationCRTMatrixZ(const float zangle) {
	const float zrot = (float)zangle * ((float)M_PI / 180.0f);
	const float s = sin(zrot);
	const float c = cos(zrot);
	return CRTMatrix(CRTVector(c, s, 0), CRTVector(-s, c, 0), CRTVector(0.0f, 0.0f, 1.0f));
}
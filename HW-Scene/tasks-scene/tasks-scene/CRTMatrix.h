#ifndef CRTMATRIX_H
#define CRTMATRIX_H

#include "CRTVector.h"
#include "utils.h"

struct CRTMatrix {
	CRTVector v0, v1, v2;

	static const CRTMatrix identity;
	static const CRTMatrix zero;

	CRTMatrix(void) { }

	CRTMatrix(const float value) : v0(value, 0, 0), v1(0, value, 0), v2(0, 0, value) { }

	CRTMatrix(const CRTVector& diagonal) : v0(diagonal.x, 0, 0), v1(0, diagonal.y, 0), v2(0, 0, diagonal.z) { }

	CRTMatrix(const CRTVector& a, const CRTVector& b, const CRTVector& c) : v0(a), v1(b), v2(c) { }

	CRTMatrix(const CRTMatrix& rhs) : v0(rhs.v0), v1(rhs.v1), v2(rhs.v2) { }

	CRTVector& operator [](const int index) {
		return (&v0)[index];
	}

	const CRTVector& operator [](const int index) const {
		return (&v0)[index];
	}

	void set(const CRTMatrix& rhs);

	void setCol(const int i, const CRTVector& a);

	CRTVector getCol(const int i) const;

	CRTMatrix transpose() const;

	CRTMatrix negate() const;

	CRTMatrix operator +(const CRTMatrix& rhs) const;

	CRTMatrix operator -(const CRTMatrix& rhs) const;

	CRTMatrix operator *(const CRTMatrix& rhs) const;

};

static const CRTMatrix identityMatrix = CRTMatrix(1.0f);
static const CRTMatrix zeroMatrix = CRTMatrix(0.0f);

CRTVector operator *(const CRTVector& a, const CRTMatrix& m);

CRTVector operator *(const CRTMatrix& m, const CRTVector& a);

CRTMatrix operator *(const CRTMatrix& m, const int x);

CRTMatrix operator *(const int x, const CRTMatrix& m);

CRTMatrix operator *(const CRTMatrix& m, const float x);

CRTMatrix operator *(const float x, const CRTMatrix& m);

CRTMatrix operator *(const CRTMatrix& m, const double& x);

CRTMatrix operator *(const double& x, const CRTMatrix& m);

// Creates a new CRTMatrix representing a linear transformation - rotation around the X axis
CRTMatrix makeRotationCRTMatrixX(const float xangle);

// Creates a new CRTMatrix representing a linear transformation - rotation around the Y axis
CRTMatrix makeRotationCRTMatrixY(const float yangle);

// Creates a new CRTMatrix representing a linear transformation - rotation around the Z axis
CRTMatrix makeRotationCRTMatrixZ(const float zangle);

#endif // !CRTMATRIX_H
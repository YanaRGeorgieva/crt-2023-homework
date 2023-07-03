#ifndef UTILS_H
#define UTILS_H

#include <cmath>
const double M_PI = 3.14159265358979323846;

const float SHADOW_BIAS = 1e-2f;
const float REFLECTION_BIAS = 1e-4f;
const float REFRACTION_BIAS = 1e-3f;

const int MAX_DEPTH = 8;

const int GI_MAX_DEPTH = 5;

const int MAX_SAMPLES = 32; // At least 1
const int MAX_DIFFUSE_REFLECTION_RAY_COUNTS = 0;

const int MAX_TRIANGLES_PER_LEAF = 10;
const int MAX_TREE_DEPTH = 64;

const int AXIS_COUNT = 3;
const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

const float EPSILON = 1e-5f;

const float MAX_FLOAT = 1e30f;
const float MIN_FLOAT = -1e30f;

//! returns if a == b, taking possible rounding errors into account
inline bool equals(const float a, const float b, const float tolerance = EPSILON) {
	return fabs(a - b) < tolerance && fabs(b - a) < tolerance;
}

//! returns if a < b, taking possible rounding errors into account
inline bool lessThan(const float a, const float b, const float tolerance = EPSILON) {
	return a < b - tolerance;
}

//! returns if a <= b, taking possible rounding errors into account
inline bool lessEqThan(const float a, const float b, const float tolerance = EPSILON) {
	return a < b - tolerance || equals(a, b);
}

//! returns if a > b, taking possible rounding errors into account
inline bool greaterThan(const float a, const float b, const float tolerance = EPSILON) {
	return a > b + tolerance;
}

//! returns if a >= b, taking possible rounding errors into account
inline bool greaterEqThan(const float a, const float b, const float tolerance = EPSILON) {
	return a > b + tolerance || equals(a, b);
}

struct CRTUintVector {
	size_t x, y, z;

	size_t& operator [](const int idx) {
		return (&x)[idx % AXIS_COUNT];
	}

	const size_t& operator [](const int idx) const {
		return (&x)[idx % AXIS_COUNT];
	}
};

inline float deg2rad(const float deg) {
	return deg * ((float)M_PI / 180.0f);
}

inline float getRandomFloatBetween0and1Inclusive() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline float getRandomFloatBetween0and1() {
	return EPSILON + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (1.0f - 2 * EPSILON));
}
#endif // !UTILS_H

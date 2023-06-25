#ifndef UTILS_H
#define UTILS_H

#include <cmath>
const double M_PI = 3.14159265358979323846;

const float SHADOW_BIAS = 1e-2f;
const float REFLECTION_BIAS = 1e-4f;
const float REFRACTION_BIAS = 1e-3f;

const int MAX_DEPTH = 7;

const int MAX_TRIANGLES_PER_LEAF = 10;

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
	return a > b - tolerance;
}

//! returns if a >= b, taking possible rounding errors into account
inline bool greaterEqThan(const float a, const float b, const float tolerance = EPSILON) {
	return a > b - tolerance || equals(a, b);
}
#endif // !UTILS_H

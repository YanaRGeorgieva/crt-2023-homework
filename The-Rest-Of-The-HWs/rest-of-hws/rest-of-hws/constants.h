#ifndef UTILS_H
#define UTILS_H

#include <cmath>
const double M_PI = 3.14159265358979323846;

const float SHADOW_BIAS = 0.01f;

const int MAX_DEPTH = 4;

const float ROUNDING_ERROR_f32 = 0.0000000001f;

//! returns if a equals b, taking possible rounding errors into account
inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_f32) {
	return fabs(a - b) < tolerance;
}

#endif // !UTILS_H

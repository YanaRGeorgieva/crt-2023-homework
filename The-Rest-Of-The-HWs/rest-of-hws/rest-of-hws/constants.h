#ifndef UTILS_H
#define UTILS_H

#include <cmath>
const double M_PI = 3.14159265358979323846;

const float SHADOW_BIAS = 1e-2f; // DO NOT TOUCH
const float REFLECTION_BIAS = 1e-4f;
const float REFRACTION_BIAS = 1e-3f;

const int MAX_DEPTH = 6;

const float ROUNDING_ERROR_f32 = 1e-5f;  // DO NOT TOUCH

//! returns if a equals b, taking possible rounding errors into account
inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_f32) {
	return fabs(a - b) < tolerance;
}

#endif // !UTILS_H

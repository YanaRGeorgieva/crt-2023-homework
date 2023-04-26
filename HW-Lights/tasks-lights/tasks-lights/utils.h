#ifndef UTILS_H
#define UTILS_H

const int maxRGBColorComponent = 255;

const double M_PI = 3.14159265358979323846;

const float ROUNDING_ERROR_f32 = 0.0000001f;
//! returns if a equals b, taking possible rounding errors into account
inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_f32) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

#endif // !UTILS_H

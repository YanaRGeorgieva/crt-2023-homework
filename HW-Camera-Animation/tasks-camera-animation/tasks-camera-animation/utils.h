#ifndef UTILS_H
#define UTILS_H

const float ROUNDING_ERROR_f32 = 0.000001f;
/// Output image resolution
const int imageWidth = 1280;
const int imageHeight = 760;
const float aspectRatio = (float)imageWidth / (float)imageHeight;
const int maxColorComponent = 255;

const double M_PI = 3.14159265358979323846;

//! returns if a equals b, taking possible rounding errors into account
inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_f32) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

struct RGBColor {
	union {
		struct { unsigned char r, g, b; };
		unsigned char rgb[3];
	};
};

#endif // !UTILS_H

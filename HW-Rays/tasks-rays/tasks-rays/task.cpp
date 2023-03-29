#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

/// Output image resolution
static const int imageWidth = 1280;
static const int imageHeight = 720;

static const float aspectRatio = (float)imageWidth / (float)imageHeight;

static const int maxColorComponent = 255;

union Color {
	struct { unsigned char r, g, b; };
	unsigned char rgb[3];
};

struct Vector {
	float x, y, z;

	Vector(float x, float y, float z) : x(x), y(y), z(z) {}

	float length() const {
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	Vector normalize() const {
		float multiplicator = 1 / this->length();
		return Vector(x * multiplicator, y * multiplicator, z * multiplicator);
	}

	Vector operator+ (const Vector& lhs) const {
		return Vector(x + lhs.x, y + lhs.y, z + lhs.z);
	}

	Vector operator- (const Vector& lhs) const {
		return Vector(x - lhs.x, y - lhs.y, z - lhs.z);
	}
};

struct Ray {
	Vector origin;
	Vector direction;
	Ray(Vector origin, Vector direction) : origin(origin), direction(direction) {};
};

Color generateCoordinateColorTask(int rowIdx, int colIdx) {
	// Find its center, based on the raster coordinates
	float x = colIdx + 0.5f;
	float y = rowIdx + 0.5f;
	// Convert raster coordinates to NDC space [0.0, 1.0]
	x /= imageWidth;
	y /= imageHeight;
	// Convert NDC coordinates to Screen space [-1.0, 1.0]
	x = (2.0f * x) - 1.0f;
	y = 1.0f - (2.0f * y);
	//Consider the aspect ratio
	x *= aspectRatio;
	// Normalaize ray direction vector
	// Store the ray with the calculated direction and origin
	Vector rayDir = Vector(x, y, -1.0f).normalize();
	Color col{};
	col.r = rayDir.x >= 0 ? rayDir.x * maxColorComponent : (rayDir.x * -1) * maxColorComponent;
	col.g = rayDir.y >= 0 ? rayDir.y * maxColorComponent : (rayDir.y * -1) * maxColorComponent;
	col.b = rayDir.z >= 0 ? rayDir.z * maxColorComponent : (rayDir.z * -1) * maxColorComponent;

	return col;
}

void writeFileTask() {
	std::ofstream ppmFileStream("task.ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;
	Color col{};
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			col = generateCoordinateColorTask(rowIdx, colIdx);
			ppmFileStream << col.r << " " << col.g << " " << col.b << "\t";
		}
		ppmFileStream << std::endl;
	}

	ppmFileStream.close();
}

int main() {
	writeFileTask();
	return 0;
}

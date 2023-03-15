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
		float length = this->length();
		return Vector(x / length, y / length, z / length);
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

char buff[24]{};
const char* generateCoordinateColorTask(int rowIdx, int colIdx) {
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
	//col.b = rayDir.z >= 0 ? rayDir.z * maxColorComponent : (rayDir.z * -1) * maxColorComponent;
	sprintf_s(buff, "%d %d %d", col.r, col.g, col.b);
	return buff;
}

void writeFileTask() {
	std::ofstream ppmFileStream("task.ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			ppmFileStream << generateCoordinateColorTask(rowIdx, colIdx) << "\t";
		}
		ppmFileStream << std::endl;
	}

	ppmFileStream.close();
}

int main() {
	writeFileTask();
	return 0;
}

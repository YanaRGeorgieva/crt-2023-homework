#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

/// Output image resolution
static const int imageWidth = 1270;
static const int imageHeight = 720;

static const int maxColorComponent = 255;

static const int widthCenter = imageWidth / 2;
static const int heightCenter = imageHeight / 2;
static const int radiusRedCircle = heightCenter / 2;
static const int radiusWhiteCircle = heightCenter / 3 + heightCenter / 108;
static const int radiusSmallRedCircle = heightCenter / 3 + heightCenter / 54;

union Color {
	struct { unsigned char r, g, b; };
	unsigned char rgb[3];
};


char buff2[24]{};
const char* generateCoordinateColorTask2(int rowIdx, int colIdx) {
	int x = rowIdx - heightCenter;
	int y = colIdx - widthCenter;

	if (x * x + y * y <= radiusRedCircle * radiusRedCircle + 1 && x * x + y * y > radiusWhiteCircle * radiusWhiteCircle + 1) {
		sprintf_s(buff2, "%d %d %d", 255, 0, 0);
	} else {
		if ((x + heightCenter / 7) * (x + heightCenter / 7) + y * y <= radiusSmallRedCircle * radiusSmallRedCircle + 1) {
			sprintf_s(buff2, "%d %d %d", 255, 0, 0);
		} else {
			sprintf_s(buff2, "%d %d %d", 232, 228, 201);
		}
	}

	return buff2;
}

void writeFileTask2() {
	std::ofstream ppmFileStream("task2.ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			ppmFileStream << generateCoordinateColorTask2(rowIdx, colIdx) << "\t";
		}
		ppmFileStream << std::endl;
	}

	ppmFileStream.close();
}

int main() {
	writeFileTask2();
	return 0;
}

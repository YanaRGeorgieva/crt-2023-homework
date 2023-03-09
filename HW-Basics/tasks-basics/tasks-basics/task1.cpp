#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

/// Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

static const int numberOfRecPerRow = 4;
static const int numberOfRecPerColumn = 4;

static const int horizontalJump = imageWidth / numberOfRecPerRow;
static const int verticalJump = imageHeight / numberOfRecPerColumn;

union Color {
	struct { unsigned char r, g, b; };
	unsigned char rgb[3];
};

Color matrix[numberOfRecPerRow * numberOfRecPerColumn];

char buff[24]{};
const char* generateCoordinateColorTask1(int rowIdx, int colIdx) {
	const int numRectRow = rowIdx / verticalJump;
	const int numRectCol = colIdx / horizontalJump;
	const int idx = numRectRow * numberOfRecPerRow + numRectCol;
	const int component = idx % 3;
	sprintf_s(buff, "%d %d %d",
		component == 0 ? (matrix[idx].r + rand()) % maxColorComponent : matrix[idx].r,
		component == 1 ? (matrix[idx].g + rand()) % maxColorComponent : matrix[idx].g,
		component == 2 ? (matrix[idx].b + rand()) % maxColorComponent : matrix[idx].b);
	return buff;
}

void writeFileTask1() {
	std::ofstream ppmFileStream("task1.ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			ppmFileStream << generateCoordinateColorTask1(rowIdx, colIdx) << "\t";
		}
		ppmFileStream << std::endl;
	}

	ppmFileStream.close();
}

void fillMatrixWithColors() {
	Color col{};
	for (size_t i = 0; i < numberOfRecPerRow; i++) {
		for (size_t j = 0; j < numberOfRecPerColumn; j++) {
			col.r = rand() % maxColorComponent;
			col.g = rand() % maxColorComponent;
			col.b = rand() % maxColorComponent;
			matrix[i * numberOfRecPerRow + j] = col;
		}
	}
}

int main() {
	srand((unsigned)time(NULL));
	fillMatrixWithColors();
	writeFileTask1();
	return 0;
}

#include "CRTImage.h"

void CRTImage::setWidthAndHeight(const size_t& imageWidth, const size_t& imageHeight) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	data.reserve(imageHeight * imageWidth);
	data.resize(imageHeight * imageWidth);
}

void CRTImage::setLeftAndTopCorners(const size_t& leftCorner, const size_t& topCorner) {
	this->leftCorner = leftCorner;
	this->topCorner = topCorner;
}

void CRTImage::setPixel(const size_t& rowIdx, const size_t& colIdx, const CRTColor& color) {
	data[rowIdx * imageWidth + colIdx] = color;
}

CRTColor CRTImage::getPixel(const size_t& rowIdx, const size_t& colIdx) const {
	return data[rowIdx * imageWidth + colIdx];
}

size_t CRTImage::getImageHeight() const {
	return imageHeight;
}

size_t CRTImage::getImageWidth() const {
	return imageWidth;
}

size_t CRTImage::getLeftCorner() const {
	return leftCorner;
}

size_t CRTImage::getTopCorner() const {
	return topCorner;
}

void CRTImage::writeToPPMFile(const std::string& filename) const {
	std::ofstream ppmFileStream(std::string(filename) + ".ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxRGBColorComponent << std::endl;

	CRTRGBColor returnColor{};
	// At each pixel :
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			returnColor = data[rowIdx * imageWidth + colIdx];
			ppmFileStream << (int)returnColor.r << " " << (int)returnColor.g << " " << (int)returnColor.b << "\t";
		}
		ppmFileStream << std::endl;
	}
	ppmFileStream.close();
}
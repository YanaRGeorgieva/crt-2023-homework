#ifndef CRTIMAGE_H
#define CRTIMAGE_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "CRTColor.h"
#include "constants.h"
#include "CRTRGBColor.h"

class CRTImage {
public:
	CRTImage(const size_t& imageWidth, const size_t& imageHeight, const size_t& leftCorner = 0, const size_t& topCorner = 0) :
		imageWidth(imageWidth), imageHeight(imageHeight) {
		data.reserve(imageHeight * imageWidth);
		data.resize(imageHeight * imageWidth);
		this->topCorner = topCorner;
		this->leftCorner = leftCorner;
	}

	void setWidthAndHeight(const size_t& imageWidth, const size_t& imageHeight);
	void setLeftAndTopCorners(const size_t& leftCorner, const size_t& topCorner);
	void setPixel(const size_t& rowIdx, const size_t& colIdx, const CRTColor& color);

	size_t getImageHeight() const;
	size_t getImageWidth() const;
	size_t getLeftCorner() const;
	size_t getTopCorner() const;
	CRTColor getPixel(const size_t& rowIdx, const size_t& colIdx) const;

	void writeToPPMFile(const std::string& filename) const;

private:
	std::vector<CRTColor> data;
	size_t imageHeight;
	size_t imageWidth;
	size_t topCorner;
	size_t leftCorner;
};

#endif // !CRTIMAGE_H
#include "CRTSettings.h"

void CRTSettings::setBGColor(const CRTVector& bgColor) {
	this->bgColor = bgColor;
}

void CRTSettings::fillImageSettings(const int imageWidth, const int imageHeight) {
	this->bgColor = bgColor;
	this->imageSettings.imageWidth = imageWidth;
	this->imageSettings.imageHeight = imageHeight;
	this->imageSettings.aspectRatio = (float)imageWidth / (float)imageHeight;
}

void CRTSettings::fillImageSettings(const CRTVector& bgColor, const int imageWidth, const int imageHeight) {
	this->imageSettings.imageWidth = imageWidth;
	this->imageSettings.imageHeight = imageHeight;
	this->imageSettings.aspectRatio = (float)imageWidth / (float)imageHeight;
}

int CRTSettings::getImageHeight() const {
	return imageSettings.imageHeight;
}
int CRTSettings::getImageWidth() const {
	return imageSettings.imageWidth;
}

float CRTSettings::getImageAspectRatio() const {
	return imageSettings.aspectRatio;
}

CRTVector CRTSettings::getBGColor() const {
	return bgColor;
}
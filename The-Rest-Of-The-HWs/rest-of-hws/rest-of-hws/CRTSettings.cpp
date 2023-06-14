#include "CRTSettings.h"

void CRTSettings::setBGColor(const CRTVector& bgColor) {
	this->bgColor = bgColor;
}

void CRTSettings::fillImageSettings(const int imageWidth, const int imageHeight, const int bucketSize) {
	this->bgColor = bgColor;
	this->imageSettings.imageWidth = imageWidth;
	this->imageSettings.imageHeight = imageHeight;
	this->imageSettings.aspectRatio = (float)imageWidth / (float)imageHeight;
	this->imageSettings.bucketSize = bucketSize;
}

void CRTSettings::fillImageSettings(const CRTVector& bgColor, const int imageWidth, const int imageHeight, const int bucketSize) {
	this->imageSettings.imageWidth = imageWidth;
	this->imageSettings.imageHeight = imageHeight;
	this->imageSettings.aspectRatio = (float)imageWidth / (float)imageHeight;
	this->imageSettings.bucketSize = bucketSize;
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

int CRTSettings::getImageBucketSize() const {
	return imageSettings.bucketSize;
}

CRTVector CRTSettings::getBGColor() const {
	return bgColor;
}
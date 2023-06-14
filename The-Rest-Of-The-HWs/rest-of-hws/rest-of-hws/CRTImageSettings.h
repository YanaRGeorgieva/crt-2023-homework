#ifndef CRTImageSettings
#define CRTImageSettings

#include "CRTVector.h"

struct ImageSettings {
	int imageWidth;
	int imageHeight;
	int bucketSize;
	float aspectRatio;

	ImageSettings(const int imageWidth = 1280, const int imageHeight = 720, const int bucketSize = 24) :
		imageWidth(imageWidth),
		imageHeight(imageHeight),
		bucketSize(bucketSize),
		aspectRatio((float)imageWidth / (float)imageHeight) { }
};

#endif // !CRTImageSettings
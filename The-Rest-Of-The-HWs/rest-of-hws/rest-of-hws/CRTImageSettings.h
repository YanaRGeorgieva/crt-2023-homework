#ifndef CRTImageSettings
#define CRTImageSettings

#include "CRTVector.h"

struct ImageSettings {
	int imageWidth;
	int imageHeight;
	int bucketSize;
	float aspectRatio;

	ImageSettings(const int imageWidth = 1920, const int imageHeight = 1200, const int bucketSize = 24) :
		imageWidth(imageWidth),
		imageHeight(imageHeight),
		bucketSize(bucketSize),
		aspectRatio((float)imageWidth / (float)imageHeight) { }
};

#endif // !CRTImageSettings
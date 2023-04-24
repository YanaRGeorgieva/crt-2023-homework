#ifndef CRTImageSettings
#define CRTImageSettings

#include "CRTVector.h"

struct ImageSettings {
	int imageWidth;
	int imageHeight;
	float aspectRatio;

	ImageSettings(const int imageWidth = 1280, const int imageHeight = 720) :
		imageWidth(imageWidth),
		imageHeight(imageHeight),
		aspectRatio((float)imageWidth / (float)imageHeight) { }
};

#endif // !CRTImageSettings
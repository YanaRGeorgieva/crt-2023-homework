#ifndef CRTSETTINGS_H
#define CRTSETTINGS_H

#include "constants.h"
#include "CRTVector.h"
#include "CRTImageSettings.h"

class CRTSettings {
public:
	friend class CRTParser;

	CRTSettings() : bgColor(zeroVector) { }

	CRTSettings(const CRTVector& bgColor, const int imageWidth = 1920, const int imageHeight = 1200, const int bucketSize = 24) :
		bgColor(bgColor),
		imageSettings(imageWidth, imageHeight, bucketSize) { }

	void setBGColor(const CRTVector& bgColor);
	void fillImageSettings(const int imageWidth, const int imageHeight, const int bucketSize = 24);
	void fillImageSettings(const CRTVector& bgColor, const int imageWidth, const int imageHeight, const int bucketSize = 24);

	int getImageHeight() const;
	int getImageWidth() const;
	float getImageAspectRatio() const;
	CRTVector getBGColor() const;
	int getImageBucketSize() const;

private:
	CRTVector bgColor;
	ImageSettings imageSettings;
};

#endif // !CRTSETTINGS_H
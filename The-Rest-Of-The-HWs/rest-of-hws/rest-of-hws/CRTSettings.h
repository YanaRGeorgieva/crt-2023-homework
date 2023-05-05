#ifndef CRTSETTINGS_H
#define CRTSETTINGS_H

#include "utils.h"
#include "CRTVector.h"
#include "CRTImageSettings.h"

class CRTSettings {
public:
	friend class CRTParser;

	CRTSettings() : bgColor(zeroVector) { }

	CRTSettings(const CRTVector& bgColor, const int imageWidth, const int imageHeight) :
		bgColor(bgColor),
		imageSettings(imageWidth, imageHeight) { }

	void setBGColor(const CRTVector& bgColor);
	void fillImageSettings(const int imageWidth, const int imageHeight);
	void fillImageSettings(const CRTVector& bgColor, const int imageWidth, const int imageHeight);

	int getImageHeight() const;
	int getImageWidth() const;
	float getImageAspectRatio() const;
	CRTVector getBGColor() const;

private:
	CRTVector bgColor;
	ImageSettings imageSettings;
};

#endif // !CRTSETTINGS_H
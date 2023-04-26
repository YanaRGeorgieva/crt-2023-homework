#ifndef CRTCAMERA_H
#define CRTCAMERA_H

#include "CRTRay.h"
#include "CRTMatrix.h"
#include "CRTVector.h"
#include "CRTImageSettings.h"

class CRTCamera {
public:
	friend class CRTParser;

	CRTCamera() : position(zeroVector), rotation(identityMatrix) { }

	CRTRay generateCameraRay(const int rowIdx, const int colIdx) const;

	void updateImageSettings(const ImageSettings& settings);

	// Truck camera movement is a type of tracking shot that moves laterally left or right.
	void truck(const CRTVector& moveDir);

	// A boom shot is a vertical camera movement achieved through the use of a crane or jib. A boom shot creates smooth, vertical camera movement by using a counter weight system.
	void boom(const CRTVector& moveDir);

	// A dolly shot moves the camera towards or away from the subject.
	void dolly(const CRTVector& moveDir);

	// Tilt camera movements direct a camera upward or downward.
	void tilt(const float degs);

	// The camera pan directs a camera horizontally left or right.
	void pan(const float degs);

	// The camera roll is a rotational camera movement that rotates the camera over its side on its long axis.
	void roll(const float degs);

private:
	CRTVector position;
	CRTMatrix rotation;
	ImageSettings cameraImageSettings;
};

#endif // !CRTCAMERA_H
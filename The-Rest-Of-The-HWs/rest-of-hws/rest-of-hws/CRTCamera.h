#ifndef CRTCAMERA_H
#define CRTCAMERA_H

#include <ctime>

#include "CRTRay.h"
#include "CRTMatrix.h"
#include "CRTVector.h"
#include "CRTImageSettings.h"

class CRTCamera {
public:
	friend class CRTParser;

	CRTCamera() : position(zeroVector), rotation(identityMatrix), fov(90.0f) {}

	CRTRay generateCameraRayRandom(const size_t& rowIdx, const size_t& colIdx, const size_t& sampleNumber) const;

	const CRTVector& getPosition() const;
	const CRTMatrix& getRotation() const;

	void updateImageSettings(const ImageSettings& settings);

	void setFov(const float fov);

	// Truck camera movement is a type of tracking shot that moves laterally left or right.
	void truck(const CRTVector& moveDir);

	// A boom shot is a vertical camera movement achieved through the use of a crane or jib. A boom shot creates smooth, vertical camera movement by using a counter weight system.
	void boom(const CRTVector& moveDir);

	// A dolly shot moves the camera towards or away from the subject.
	void dolly(const CRTVector& moveDir);

	// Tilt camera movements direct a camera upward or downward. Clamped in [-90, 90].
	void tilt(const float degs);

	// The camera pan directs a camera horizontally left or right. Clamped in (0, 180).
	void pan(const float degs);

	// The camera roll is a rotational camera movement that rotates the camera over its side on its long axis.
	void roll(const float degs);

	void turnAroundAxis(const CRTMatrix& initialRotation, const CRTVector& initialPosition, const float degs, const int axis = X_AXIS);
private:
	CRTVector position;
	CRTMatrix rotation;
	float fov;
	ImageSettings cameraImageSettings;
};

#endif // !CRTCAMERA_H
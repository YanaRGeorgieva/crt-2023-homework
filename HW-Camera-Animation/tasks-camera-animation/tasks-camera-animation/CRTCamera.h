#ifndef CRTCAMERA_H
#define CRTCAMERA_H

#include "utils.h"
#include "CRTVector.h"
#include "CRTRay.h"
#include "CRTMatrix.h"

struct CRTCamera {
	CRTVector position = zeroVector;
	CRTMatrix rotation = identityMatrix;

	CRTRay generateCameraRay(const int rowIdx, const int colIdx) {
		// Find its center, based on the raster coordinates
		float x = colIdx + 0.5f;
		float y = rowIdx + 0.5f;
		// Convert raster coordinates to NDC space [0.0, 1.0]
		x /= imageWidth;
		y /= imageHeight;
		// Convert NDC coordinates to Screen space [-1.0, 1.0]
		x = (2.0f * x) - 1.0f;
		y = 1.0f - (2.0f * y);
		//Consider the aspect ratio
		x *= aspectRatio;
		// Normalaize ray direction vector
		CRTVector rayDir((CRTVector(x, y, -1.0f) * rotation).normalize());
		// Return the ray with the calculated direction and origin
		return CRTRay(position, rayDir);
	}

	// Truck camera movement is a type of tracking shot that moves laterally left or right.
	void truck(const CRTVector& moveDir) {
		CRTVector moveDirWorldSpace = moveDir * CRTVector(-1, 0, 0);
		moveDirWorldSpace = moveDirWorldSpace * rotation;
		position = position + moveDirWorldSpace;
	}

	// A boom shot is a vertical camera movement achieved through the use of a crane or jib. A boom shot creates smooth, vertical camera movement by using a counter weight system.
	void boom(const CRTVector& moveDir) {
		CRTVector moveDirWorldSpace = moveDir * CRTVector(0, -1, 0);
		moveDirWorldSpace = moveDirWorldSpace * rotation;
		position = position + moveDirWorldSpace;
	}

	// A dolly shot moves the camera towards or away from the subject.
	void dolly(const CRTVector& moveDir) {
		CRTVector moveDirWorldSpace = moveDir * CRTVector(0, 0, 1);
		moveDirWorldSpace = moveDirWorldSpace * rotation;
		position = position + moveDirWorldSpace;
	}

	// Tilt camera movements direct a camera upward or downward.
	void tilt(const float degs) {
		const CRTMatrix xRotation = makeRotationCRTMatrixX(degs);
		rotation = rotation * xRotation;
	}

	// The camera pan directs a camera horizontally left or right.
	void pan(const float degs) {
		const CRTMatrix yRotation = makeRotationCRTMatrixY(degs);
		rotation = rotation * yRotation;
	}

	// The camera roll is a rotational camera movement that rotates the camera over its side on its long axis.
	void roll(const float degs) {
		const CRTMatrix zRotation = makeRotationCRTMatrixZ(degs);
		rotation = rotation * zRotation;
	}
};

#endif // !CRTCAMERA_H
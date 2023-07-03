#include "CRTCamera.h"

CRTRay CRTCamera::generateCameraRayRandom(const size_t& rowIdx, const size_t& colIdx, const size_t& sampleNumber) const {
	float x = colIdx;
	float y = rowIdx;
	if (sampleNumber == 0) {
		// Find its center, based on the raster coordinates
		x += 0.5f;
		y += 0.5f;
	} else {
		x += getRandomFloatBetween0and1();
		y += getRandomFloatBetween0and1();
	}
	// Convert raster coordinates to NDC space [0.0, 1.0]
	x /= cameraImageSettings.imageWidth;
	y /= cameraImageSettings.imageHeight;
	// Convert NDC coordinates to Screen space [-1.0, 1.0]
	x = (2.0f * x) - 1.0f;
	y = 1.0f - (2.0f * y);
	//Consider the aspect ratio
	x *= cameraImageSettings.aspectRatio;
	// Apply fov
	x *= tanf(deg2rad(fov / 2.0f));
	y *= tanf(deg2rad(fov / 2.0f));
	// Normalaize ray direction vector
	CRTVector rayDir((CRTVector(x, y, -1.0f) * rotation).normalize());
	// Return the ray with the calculated direction and origin
	return CRTRay(position, rayDir);
}

void CRTCamera::updateImageSettings(const ImageSettings& settings) {
	cameraImageSettings.imageWidth = settings.imageWidth;
	cameraImageSettings.imageHeight = settings.imageHeight;
	cameraImageSettings.aspectRatio = settings.aspectRatio;
}

const CRTVector& CRTCamera::getPosition() const {
	return position;
}

void CRTCamera::setFov(const float fov) {
	this->fov = fov;
}

const CRTMatrix& CRTCamera::getRotation() const {
	return rotation;
}

void CRTCamera::truck(const CRTVector& moveDir) {
	CRTVector moveDirWorldSpace = moveDir * CRTVector(-1.0f, 0.0f, 0.0f);
	moveDirWorldSpace = moveDirWorldSpace * rotation;
	position = position + moveDirWorldSpace;
}

void CRTCamera::boom(const CRTVector& moveDir) {
	CRTVector moveDirWorldSpace = moveDir * CRTVector(0.0f, -1.0f, 0.0f);
	moveDirWorldSpace = moveDirWorldSpace * rotation;
	position = position + moveDirWorldSpace;
}

void CRTCamera::dolly(const CRTVector& moveDir) {
	CRTVector moveDirWorldSpace = moveDir * CRTVector(0.0f, 0.0f, -1.0f);
	moveDirWorldSpace = moveDirWorldSpace * rotation;
	position = position + moveDirWorldSpace;
}

void CRTCamera::tilt(const float degs) {
	const CRTMatrix xRotation = makeRotationCRTMatrixX(fminf(fmaxf(degs, -90.0f), 90.0f));
	rotation = rotation * xRotation;
}

void CRTCamera::pan(const float degs) {
	const CRTMatrix yRotation = makeRotationCRTMatrixY(fminf(fmaxf(degs, EPSILON), 180.0f - EPSILON));
	rotation = rotation * yRotation;
}

void CRTCamera::roll(const float degs) {
	const CRTMatrix zRotation = makeRotationCRTMatrixZ(degs);
	rotation = rotation * zRotation;
}

void CRTCamera::turnAroundAxis(const CRTMatrix& initialRotation, const CRTVector& initialPosition, const float degs, const int axis) {
	CRTMatrix turnRotation{};
	switch (axis) {
	case X_AXIS:
		turnRotation = makeRotationCRTMatrixX(degs);
		break;
	case Y_AXIS:
		turnRotation = makeRotationCRTMatrixY(degs);
		break;
	case Z_AXIS:
		turnRotation = makeRotationCRTMatrixZ(degs);
		break;
	default:
		turnRotation = identityMatrix;
		break;
	}
	rotation = initialRotation * turnRotation;
	position = initialPosition * turnRotation;
}
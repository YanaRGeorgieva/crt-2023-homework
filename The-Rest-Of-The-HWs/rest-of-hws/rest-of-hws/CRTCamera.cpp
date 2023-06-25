#include "CRTCamera.h"

CRTRay CRTCamera::generateCameraRay(const size_t& rowIdx, const size_t& colIdx) const {
	// Find its center, based on the raster coordinates
	float x = colIdx + 0.5f;
	float y = rowIdx + 0.5f;
	// Convert raster coordinates to NDC space [0.0, 1.0]
	x /= cameraImageSettings.imageWidth;
	y /= cameraImageSettings.imageHeight;
	// Convert NDC coordinates to Screen space [-1.0, 1.0]
	x = (2.0f * x) - 1.0f;
	y = 1.0f - (2.0f * y);
	//Consider the aspect ratio
	x *= cameraImageSettings.aspectRatio;
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
	CRTVector moveDirWorldSpace = moveDir * CRTVector(0.0f, 0.0f, 1.0f);
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
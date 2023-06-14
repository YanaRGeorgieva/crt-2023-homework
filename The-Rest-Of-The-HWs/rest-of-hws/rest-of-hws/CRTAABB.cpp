#include "CRTAABB.h"

bool CRTAABB::isValid() const {
	return boxMax.x - boxMin.x > ROUNDING_ERROR_f32
		&& boxMax.y - boxMin.y > ROUNDING_ERROR_f32
		&& boxMax.z - boxMin.z > ROUNDING_ERROR_f32;
}

void CRTAABB::expand(const CRTVector& vertex) {
	boxMin.x = std::fminf(boxMin.x, vertex.x);
	boxMax.x = std::fmaxf(boxMax.x, vertex.x);
	boxMin.y = std::fminf(boxMin.y, vertex.y);
	boxMax.y = std::fmaxf(boxMax.y, vertex.y);
	boxMin.z = std::fminf(boxMin.z, vertex.z);
	boxMax.z = std::fmaxf(boxMax.z, vertex.z);
}

retIntersectionBox CRTAABB::intersect(const CRTRay& ray, float& thresholdT) const {
	retIntersectionBox returnData;
	returnData.isValid = false;

	const CRTVector invDirection = CRTVector(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);

	// Check for intersection on X side
	float tMinX = (boxMin.x - ray.origin.x) * invDirection.x;
	if (tMinX > 0 && tMinX < thresholdT) {
		CRTVector pMinX = ray.origin + tMinX * ray.direction;
		if (pMinX.y >= boxMin.y && pMinX.y <= boxMax.y &&
			pMinX.z >= boxMin.z && pMinX.z <= boxMax.z) {
			returnData.isValid = true;
			returnData.p = pMinX;
			returnData.t = tMinX;
			return returnData;
		}
	}

	float tMaxX = (boxMax.x - ray.origin.x) * invDirection.x;
	if (tMaxX > 0 && tMaxX < thresholdT) {
		CRTVector pMaxX = ray.origin + tMaxX * ray.direction;
		if (pMaxX.y >= boxMin.y && pMaxX.y <= boxMax.y &&
			pMaxX.z >= boxMin.z && pMaxX.z <= boxMax.z) {
			returnData.isValid = true;
			returnData.p = pMaxX;
			returnData.t = tMaxX;
			return returnData;
		}
	}

	// Check for intersection on Y side
	float tMinY = (boxMin.y - ray.origin.y) * invDirection.y;
	if (tMinY > 0 && tMinY < thresholdT) {
		CRTVector pMinY = ray.origin + tMinY * ray.direction;
		if (pMinY.x >= boxMin.x && pMinY.x <= boxMax.x &&
			pMinY.z >= boxMin.z && pMinY.z <= boxMax.z) {
			returnData.isValid = true;
			returnData.p = pMinY;
			returnData.t = tMinY;
			return returnData;
		}
	}

	float tMaxY = (boxMax.y - ray.origin.y) * invDirection.y;
	if (tMaxY > 0 && tMaxY < thresholdT) {
		CRTVector pMaxY = ray.origin + tMaxY * ray.direction;
		if (pMaxY.x >= boxMin.x && pMaxY.x <= boxMax.x &&
			pMaxY.z >= boxMin.z && pMaxY.z <= boxMax.z) {
			returnData.isValid = true;
			returnData.p = pMaxY;
			returnData.t = tMaxY;
			return returnData;
		}
	}

	// Check for intersection on Z side
	float tMinZ = (boxMin.z - ray.origin.z) * invDirection.z;
	if (tMinZ > 0 && tMinZ < thresholdT) {
		CRTVector pMinZ = ray.origin + tMinZ * ray.direction;
		if (pMinZ.x >= boxMin.x && pMinZ.x <= boxMax.x &&
			pMinZ.y >= boxMin.y && pMinZ.y <= boxMax.y) {
			returnData.isValid = true;
			returnData.p = pMinZ;
			returnData.t = tMinZ;
			return returnData;
		}
	}

	float tMaxZ = (boxMax.z - ray.origin.z) * invDirection.z;
	if (tMaxZ > 0 && tMaxZ < thresholdT) {
		CRTVector pMaxZ = ray.origin + tMaxZ * ray.direction;
		if (pMaxZ.x >= boxMin.x && pMaxZ.x <= boxMax.x &&
			pMaxZ.y >= boxMin.y && pMaxZ.y <= boxMax.y) {
			returnData.isValid = true;
			returnData.p = pMaxZ;
			returnData.t = tMaxZ;
			return returnData;
		}
	}
	return returnData;
}
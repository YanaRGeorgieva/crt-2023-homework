#include "CRTBox.h"

bool CRTBox::isValid() const {
	return boxMax.x - boxMin.x > EPSILON
		&& boxMax.y - boxMin.y > EPSILON
		&& boxMax.z - boxMin.z > EPSILON;
}

void CRTBox::expand(const CRTVector& vertex) {
	for (int axis = 0; axis < AXIS_COUNT; axis++) {
		boxMin[axis] = std::fminf(boxMin[axis], vertex[axis]);
		boxMax[axis] = std::fmaxf(boxMax[axis], vertex[axis]);
	}
}

retIntersectionBox CRTBox::intersect(const CRTRay& ray, const float& thresholdT) const {
	retIntersectionBox returnData;
	returnData.isValid = false;

	const CRTVector invDirection = CRTVector(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);

	for (int axis = 0; axis < AXIS_COUNT; axis++) {
		// Check for intersection on axis side
		float tMinX = (boxMin[axis] - ray.origin[axis]) * invDirection[axis];
		if (greaterThan(tMinX, 0.0f) && lessThan(tMinX, thresholdT)) {
			CRTVector pMinX = ray.origin + tMinX * ray.direction;
			if (greaterEqThan(pMinX[axis + 1], boxMin[axis + 1]) && lessEqThan(pMinX[axis + 1], boxMax[axis + 1]) &&
				greaterEqThan(pMinX[axis + 2], boxMin[axis + 2]) && lessEqThan(pMinX[axis + 2], boxMax[axis + 2])) {
				returnData.isValid = true;
				returnData.p = pMinX;
				returnData.t = tMinX;
				return returnData;
			}
		}

		float tMaxX = (boxMax[axis] - ray.origin[axis]) * invDirection[axis];
		if (greaterThan(tMaxX, 0.0f) && lessThan(tMaxX, thresholdT, 1e-3f)) {
			CRTVector pMaxX = ray.origin + tMaxX * ray.direction;
			if (greaterEqThan(pMaxX[axis + 1], boxMin[axis + 1]) && lessEqThan(pMaxX[axis + 1], boxMax[axis + 1]) &&
				greaterEqThan(pMaxX[axis + 2], boxMin[axis + 2]) && lessEqThan(pMaxX[axis + 2], boxMax[axis + 2])) {
				returnData.isValid = true;
				returnData.p = pMaxX;
				returnData.t = tMaxX;
				return returnData;
			}
		}
	}

	return returnData;
}
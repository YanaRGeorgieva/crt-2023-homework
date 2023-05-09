#include "CRTRenderer.h"

CRTColor CRTRenderer::intersectScene(const CRTRay& ray) const {
	return intersectRayWithObjectsInScene(ray, scene->getGeometryObjects());
}

CRTImage CRTRenderer::render(const int maxNumberOfBuckets) const {
	const int imageWidth = scene->getSettings().getImageWidth();
	const int imageHeight = scene->getSettings().getImageHeight();

	const int sqrtNumBuckets = (int)sqrt(maxNumberOfBuckets);

	const int subWidth = imageWidth / sqrtNumBuckets;
	const int subHeight = imageHeight / sqrtNumBuckets;

	const int numHorizontal = imageWidth / subWidth;
	const int numVertical = imageHeight / subHeight;

	CRTImage image(imageWidth, imageHeight);
	std::vector<CRTImage> subImages;
	subImages.reserve(maxNumberOfBuckets);

	for (size_t i = 0; i < numVertical; i++) {
		for (size_t j = 0; j < numHorizontal; j++) {
			// Calculate the top-left and bottom-right pixel coordinates for this sub-image
			size_t top = i * subHeight;
			size_t realHeight = subHeight;
			if (top + subHeight > imageHeight) {
				realHeight = imageHeight - top;
			}
			size_t left = j * subWidth;
			size_t realWidth = subWidth;
			if (left + subWidth > imageWidth) {
				realWidth = imageWidth - left;
			}
			CRTImage subImage(realWidth, realHeight, left, top);
			subImages.emplace_back(subImage);
		}
	}

	const int length = numVertical * numHorizontal;
#pragma omp parallel for
	for (int i = 0; i < length; i++) {
		processSubimage(subImages[i]);
	}

	// At each pixel :
	for (size_t i = 0; i < length; i++) {
		const CRTImage& subImage = subImages[i];
		const size_t& height = subImage.getImageHeight();
		const size_t& width = subImage.getImageWidth();
		const size_t& left = subImage.getLeftCorner();
		const size_t& top = subImage.getTopCorner();
		// At each pixel :
		for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
			for (int colIdx = 0; colIdx < width; ++colIdx) {
				image.setPixel(top + rowIdx, left + colIdx, subImage.getPixel(rowIdx, colIdx));
			}
		}
	}

	return image;
}

void CRTRenderer::processSubimage(CRTImage& subImage) const {
	const size_t& height = subImage.getImageHeight();
	const size_t& width = subImage.getImageWidth();
	const size_t& left = subImage.getLeftCorner();
	const size_t& top = subImage.getTopCorner();
	// At each pixel :
	for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
		for (int colIdx = 0; colIdx < width; ++colIdx) {
			// Generate camera ray R : 3rd Lecture taking into account the camera position
			const size_t rIdx = top + rowIdx;
			const size_t cIdx = left + colIdx;
			CRTRay cameraRay = scene->getCamera().generateCameraRay(rIdx, cIdx);
			subImage.setPixel(rowIdx, colIdx, intersectScene(cameraRay));
		}
	}
}

CRTColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray,
	const std::vector<CRTMesh>& geometryObjects,
	int depth) const {

	if (depth > MAX_DEPTH) {
		return scene->getSettings().getBGColor();
	}

	InformationIntersectionPoint intersectionPoint{};
	InformationIntersectionPoint bestIntersectionPointInfo{};
	bestIntersectionPointInfo.p = worstP;
	bestIntersectionPointInfo.isValid = false;

	float bestT{ std::numeric_limits<float>::max() };

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectionPoint = intersectRayWithAnObject(ray, i, geometryObjects[i], bestT);
		if (intersectionPoint.isValid) {
			bestIntersectionPointInfo = intersectionPoint;
		}
	}

	if (!bestIntersectionPointInfo.isValid) {
		return scene->getSettings().getBGColor();
	}

	const std::vector<CRTMaterial>& materials = scene->getMaterials();
	const CRTMaterial& material = materials[bestIntersectionPointInfo.materialIndex];

	CRTVector shadeNormal = bestIntersectionPointInfo.triN;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPointInfo.hitNormal;
	}

	CRTColor returnCol{};
	if (material.type == CRTMaterial::DIFFUSE) {
		returnCol = shade(bestIntersectionPointInfo, shadeNormal, material);
	}
	else if (material.type == CRTMaterial::REFLECTIVE) {
		// Create reflectionRay
		CRTVector Y = ray.direction.dot(shadeNormal) * shadeNormal;
		CRTVector X = ray.direction - ray.direction.dot(shadeNormal) * shadeNormal;
		CRTRay reflectionRay(bestIntersectionPointInfo.p + shadeNormal * SHADOW_BIAS, X - Y);
		CRTColor reflectionColor = intersectRayWithObjectsInScene(reflectionRay, geometryObjects, depth + 1);
		returnCol = reflectionColor * material.albedo;
	}
	return returnCol;
}

bool CRTRenderer::hasIntersectRayWithObjectsInScene(const CRTRay& ray,
	const std::vector<CRTMesh>& geometryObjects,
	const float lightLength) const {

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		const std::vector<CRTTriangle>& triangles = geometryObjects[i].getTriangles();
		const size_t len = triangles.size();
		for (size_t i = 0; i < len; i++) {
			const CRTTriangle::retDataFromTriIntersect& intersectData = triangles[i].intersect(ray, lightLength);
			//	If P is on the left of the 3 edges and t > 0, we have an intersection
			if (lightLength > intersectData.t) {
				return true;
			}
		}
	}
	return false;
}

CRTColor CRTRenderer::shade(const CRTRenderer::InformationIntersectionPoint& bestIntersectionPoint,
	const CRTVector& shadeNormal,
	const CRTMaterial& material) const {

	CRTColor finalColor{};
	const std::vector<CRTLight>& lights = scene->getLights();
	const size_t lightsCnt = lights.size();
	if (lightsCnt == 0) {
		// For the triangle scene
		return bestIntersectionPoint.barycentricCoordinates;
	}
	// For all lights in the scene:
	for (size_t i = 0; i < lightsCnt; i++) {
		// Compute the direction from p to the light position:
		CRTVector lightDir = lights[i].position - bestIntersectionPoint.p;
		// Compute sphere radius:
		float sr = lightDir.length();
		// Normalize lightDir
		CRTVector normLightDir = lightDir.normalize();
		// Calculate the Cosine Law:
		float cosLaw = normLightDir.dot(shadeNormal);
		if (cosLaw < 0.0f) { continue; }
		// Compute sphere area
		float sa = 4 * (float)M_PI * sr * sr;
		// Create shadowRay
		CRTRay shadowRay(bestIntersectionPoint.p + shadeNormal * SHADOW_BIAS, normLightDir);
		//Trace shadowRay to check for triangle intersection
		bool hasIntersection = hasIntersectRayWithObjectsInScene(shadowRay, scene->getGeometryObjects(), sr);
		CRTColor lightContribution = hasIntersection ? CRTColor{ } :
			CRTColor(lights[i].intensity / sa * material.albedo * cosLaw);
		finalColor += lightContribution;
	}
	return finalColor;
}

CRTRenderer::InformationIntersectionPoint CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
	const size_t idxGeometryObject,
	const CRTMesh& geometryObject,
	float& bestT) const {

	const std::vector<CRTTriangle>& triangles = geometryObject.getTriangles();
	const std::vector<size_t>& faces = geometryObject.getFaces();
	const std::vector<CRTVector>& vertexNormals = geometryObject.getVertexNormals();

	InformationIntersectionPoint intersectionPointInfo{};
	intersectionPointInfo.isValid = false;

	const size_t len = triangles.size();
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const CRTTriangle::retDataFromTriIntersect& intersectData = tri.intersect(ray, bestT);
		//	If P is on the left of the 3 edges and t > 0, we have an intersection
		if (bestT > intersectData.t) {
			// Get closest p (with least length)
			bestT = intersectData.t;
			intersectionPointInfo.set(idxGeometryObject, i, tri, intersectData, geometryObject, vertexNormals, faces);
		}
	}
	return intersectionPointInfo;
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
}

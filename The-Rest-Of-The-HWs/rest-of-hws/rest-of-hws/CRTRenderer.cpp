#include "CRTRenderer.h"

CRTColor CRTRenderer::intersectScene(const CRTRay& ray) const {
	return intersectRayWithObjectsInScene(ray, scene->getGeometryObjects());
}

void CRTRenderer::setAlbedo(const CRTColor& albedo) {
	this->albedo = albedo;
}

void CRTRenderer::setShadowBias(const float shadowBias) {
	this->shadowBias = shadowBias;
}

CRTImage CRTRenderer::render(const int maxNumberOfBuckets) const {
	const int imageWidth = scene->getSettings().getImageWidth();
	const int imageHeight = scene->getSettings().getImageHeight();

	const int subWidth = imageWidth / sqrt(maxNumberOfBuckets);
	const int subHeight = imageHeight / sqrt(maxNumberOfBuckets);

	int numHorizontal = imageWidth / subWidth;
	int numVertical = imageHeight / subHeight;

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

	omp_set_num_threads(numHorizontal);

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

CRTColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const {

	InformationIntersectionPoint intersectionPoint{};
	InformationIntersectionPoint bestIntersectionPoint{};
	bestIntersectionPoint.p = worstP;
	bestIntersectionPoint.isValid = false;

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectionPoint = intersectRayWithAnObject(ray, i, geometryObjects[i]);
		if (intersectionPoint.isValid && intersectionPoint < bestIntersectionPoint) {
			bestIntersectionPoint = intersectionPoint;
		}
	}

	if (!bestIntersectionPoint.isValid) {
		return scene->getSettings().getBGColor();
	}

	const std::vector<CRTLight>& lights = scene->getLights();
	const size_t lightsCnt = lights.size();

	CRTColor returnCol{};

	// For all lights in the scene:
	for (size_t i = 0; i < lightsCnt; i++) {
		// Compute the direction from p to the light position:
		CRTVector lightDir = lights[i].position - bestIntersectionPoint.p;
		// Compute sphere radius:
		float sr = lightDir.length();
		// Normilize lightDir
		CRTVector normLightDir = lightDir.normalize();
		// Calculate the Cosine Law:
		float tmpCosLaw = normLightDir.dot(bestIntersectionPoint.triN);
		float cosLaw = tmpCosLaw < 0.0f ? 0.0f : tmpCosLaw;
		// Light intensity:
		// Albedo: 
		// Compute shere area
		float sa = 4 * (float)M_PI * sr * sr;
		// Create shadowRay
		CRTRay shadowRay(bestIntersectionPoint.p + bestIntersectionPoint.triN * shadowBias, normLightDir);
		//Trace shadowRay to check for triangle intersection
		bool hasIntersection = hasIntersectRayWithObjectsInScene(shadowRay, scene->getGeometryObjects());
		CRTColor lightContribution = hasIntersection ? CRTColor{ } : CRTColor((lights[i].intensity / sa) * albedo * cosLaw);
		returnCol += lightContribution;
	}

	return returnCol;
}


bool CRTRenderer::hasIntersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const {

	InformationIntersectionPoint intersectionPoint{};
	InformationIntersectionPoint bestIntersectionPoint{};
	bestIntersectionPoint.p = worstP;
	bestIntersectionPoint.isValid = false;

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectionPoint = intersectRayWithAnObject(ray, i, geometryObjects[i]);
		if (intersectionPoint.isValid && intersectionPoint < bestIntersectionPoint) {
			bestIntersectionPoint = intersectionPoint;
		}
	}

	return bestIntersectionPoint.isValid;
}


CRTRenderer::InformationIntersectionPoint CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
																				const size_t idxGeometryObject,
																				const CRTMesh& geometryObject) const {
	CRTVector bestP{ worstP };

	const std::vector<CRTTriangle>& triangles = geometryObject.getTriangles();
	const std::vector<CRTVector>& colors = geometryObject.getColors();

	InformationIntersectionPoint intersectionPoint{};
	intersectionPoint.isValid = false;

	const size_t len = triangles.size();
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const CRTVector& p = tri.intersect(ray);
		//	If P is on the left of the 3 edges and t > 0, we have an intersection
		if (bestP.length() > p.length()) {
			// Get closest p (with least length)
			bestP = p;
			intersectionPoint.idxGeometry = idxGeometryObject;
			intersectionPoint.idxTriangle = i;
			intersectionPoint.p = p;
			intersectionPoint.triN = tri.getNormalVector();
			intersectionPoint.color = colors[i];
			intersectionPoint.isValid = true;
		}
	}

	return intersectionPoint;
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
}

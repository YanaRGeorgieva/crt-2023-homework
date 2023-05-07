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

CRTColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects, int depth) const {
	if (depth > MAX_DEPTH) {
		return scene->getSettings().getBGColor();
	}

	InformationIntersectionPoint intersectionPoint{};
	InformationIntersectionPoint bestIntersectionPoint{};
	bestIntersectionPoint.p = worstP;
	bestIntersectionPoint.isValid = false;

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectionPoint = intersectRayWithAnObject(ray, i, geometryObjects[i]);
		if (intersectionPoint.isValid &&
			(bestIntersectionPoint.p - ray.origin).length() > (intersectionPoint.p - ray.origin).length()) {
			bestIntersectionPoint = intersectionPoint;
		}
	}

	if (!bestIntersectionPoint.isValid) {
		return scene->getSettings().getBGColor();
	}

	const std::vector<CRTMaterial>& materials = scene->getMaterials();
	const CRTMaterial& material = materials[bestIntersectionPoint.materialIndex];

	CRTVector normal = bestIntersectionPoint.triN;
	if (material.isSmoothShaded) {
		normal = bestIntersectionPoint.hitNormal;
	}

	CRTColor returnCol{};
	if (material.type == "diffuse") {
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
			// Normilize lightDir
			CRTVector normLightDir = lightDir.normalize();
			// Calculate the Cosine Law:
			float tmpCosLaw = normLightDir.dot(normal);
			float cosLaw = tmpCosLaw < 0.0f ? 0.0f : tmpCosLaw;
			// Compute shere area
			float sa = 4 * (float)M_PI * sr * sr;
			// Create shadowRay
			CRTRay shadowRay(bestIntersectionPoint.p + normal * SHADOW_BIAS, normLightDir);
			//Trace shadowRay to check for triangle intersection
			bool hasIntersection = hasIntersectRayWithObjectsInScene(shadowRay, scene->getGeometryObjects());
			CRTColor lightContribution;
			lightContribution = hasIntersection ? CRTColor{ } : CRTColor(lights[i].intensity / sa * material.albedo * cosLaw);
			returnCol += lightContribution;
		}
	}
	else if (material.type == "reflective") {
		// Create reflectionRay
		CRTVector Y = ray.direction.dot(normal) * normal;
		CRTVector X = ray.direction - ray.direction.dot(normal) * normal;
		CRTRay reflectionRay(bestIntersectionPoint.p + normal * SHADOW_BIAS, X - Y);
		CRTColor reflectionColor = intersectRayWithObjectsInScene(reflectionRay, geometryObjects, depth + 1);
		returnCol = reflectionColor * material.albedo;
	}
	return returnCol;
}

bool CRTRenderer::hasIntersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const {
	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		const std::vector<CRTTriangle>& triangles = geometryObjects[i].getTriangles();
		const size_t len = triangles.size();
		for (size_t i = 0; i < len; i++) {
			const CRTTriangle& tri = triangles[i];
			const std::pair<CRTVector, CRTVector>& pAndBarycentricCoords = tri.intersect(ray);
			//	If P is on the left of the 3 edges and t > 0, we have an intersection
			if ((worstP - ray.origin).length() > (pAndBarycentricCoords.first - ray.origin).length()) {
				return true;
			}
		}
	}
	return false;
}


CRTRenderer::InformationIntersectionPoint CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
	const size_t idxGeometryObject,
	const CRTMesh& geometryObject) const {
	CRTVector bestP{ worstP };

	const std::vector<CRTTriangle>& triangles = geometryObject.getTriangles();
	const std::vector<size_t>& faces = geometryObject.getFaces();
	const std::vector<CRTVector>& vertexNormals = geometryObject.getVertexNormals();

	InformationIntersectionPoint intersectionPoint{};
	intersectionPoint.isValid = false;

	const size_t len = triangles.size();
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const std::pair<CRTVector, CRTVector>& pAndBarycentricCoords = tri.intersect(ray);
		//	If P is on the left of the 3 edges and t > 0, we have an intersection
		if ((bestP - ray.origin).length() > (pAndBarycentricCoords.first - ray.origin).length()) {
			// Get closest p (with least length)
			bestP = pAndBarycentricCoords.first;
			intersectionPoint.idxGeometry = idxGeometryObject;
			intersectionPoint.idxTriangle = i;
			intersectionPoint.p = pAndBarycentricCoords.first;
			intersectionPoint.triN = tri.getFaceNormal();
			const size_t idx = i * 3;
			intersectionPoint.hitNormal =
				(vertexNormals[faces[idx + 1]] * pAndBarycentricCoords.second.x +
					vertexNormals[faces[idx + 2]] * pAndBarycentricCoords.second.y +
					vertexNormals[faces[idx]] * pAndBarycentricCoords.second.z).normalize();
			intersectionPoint.barycentricCoordinates = pAndBarycentricCoords.second;
			intersectionPoint.isValid = true;
			intersectionPoint.materialIndex = geometryObject.getMaterialIdx();
		}
	}
	return intersectionPoint;
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
}

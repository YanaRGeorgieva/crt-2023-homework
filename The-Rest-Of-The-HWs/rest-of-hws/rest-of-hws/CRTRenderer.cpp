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

bool CRTRenderer::hasIntersectRayWithObjectsInScene(const CRTRay& ray,
	const std::vector<CRTMesh>& geometryObjects,
	const float thresholdPminusLight) const {

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		const std::vector<CRTTriangle>& triangles = geometryObjects[i].getTriangles();
		const size_t len = triangles.size();
		for (size_t i = 0; i < len; i++) {
			const CRTTriangle::retDataFromTriIntersect& intersectData = 
				triangles[i].intersect(ray, thresholdPminusLight);
			//	If P is on the left of the 3 edges and t > 0, we have an intersection
			if (thresholdPminusLight > intersectData.t) {
				return true;
			}
		}
	}
	return false;
}

CRTIntersectionData CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
	const size_t idxGeometryObject,
	const CRTMesh& geometryObject,
	float& bestT) const {

	const std::vector<CRTTriangle>& triangles = geometryObject.getTriangles();
	const std::vector<size_t>& faces = geometryObject.getFaces();
	const std::vector<CRTVector>& vertexNormals = geometryObject.getVertexNormals();

	CRTIntersectionData intersectionPointInfo{};
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

CRTColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray,
	const std::vector<CRTMesh>& geometryObjects) const {

	CRTIntersectionData intersectionPoint{};
	CRTIntersectionData bestIntersectionPointInfo{};
	bestIntersectionPointInfo.isValid = false;

	float bestT{ std::numeric_limits<float>::max() };

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectionPoint = intersectRayWithAnObject(ray, i, geometryObjects[i], bestT);
		if (intersectionPoint.isValid) {
			bestIntersectionPointInfo = intersectionPoint;
		}
	}

	return shade(ray, bestIntersectionPointInfo);
}

CRTColor CRTRenderer::shade(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const{

	CRTColor resultColor(0.0f);

	const CRTMaterial::MaterialTypes hitMtlType{ bestIntersectionPoint.material.type };

	if (ray.pathDepth >= MAX_DEPTH || !bestIntersectionPoint.isValid) {
		return scene->getSettings().getBGColor();
	} else if (CRTMaterial::MaterialTypes::diffuse == hitMtlType) {
		resultColor = shadeDiffuse(ray, bestIntersectionPoint);
	} else if (CRTMaterial::MaterialTypes::reflective == hitMtlType) {
		resultColor = shadeReflective(ray, bestIntersectionPoint);
	} else if (CRTMaterial::MaterialTypes::refractive == hitMtlType) {
		resultColor = shadeRefractive(ray, bestIntersectionPoint);
	} else if (CRTMaterial::MaterialTypes::constant == hitMtlType) {
		resultColor = shadeConstant(ray, bestIntersectionPoint);
	} else {
		assert(false);
	}

	return resultColor;
}

CRTColor CRTRenderer::shadeConstant(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {
	return bestIntersectionPoint.material.albedo;
}

CRTColor CRTRenderer::shadeRefractive(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {
	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

	CRTVector normal = shadeNormal;
	float currentMaterialIndex{};
	float newMaterialIndex{};
	float eta1{};
	float eta2{};
	float one80MinusCosAlpha = ray.direction.dot(normal);
	// Check if the incident ray leaves the transparent object:
	if (one80MinusCosAlpha > 0) {
		// Then we need N = -N and swap(eta1, eta2):
		normal = -1 * normal;
		float tmp = eta1;
		eta1 = eta2;
		eta2 = tmp;
	}
	// Compute cosine between I and N (cos(180-alpha) = -cos(alpha)):
	const float cosAlpha = -one80MinusCosAlpha;
	// Using Snell's Law find sin(R, -N):
	const float sinBeta = (sqrtf(1 - cosAlpha * cosAlpha) * eta1) / eta2;
	// Compute vector R using vector addition:
	const CRTVector C = (ray.direction + cosAlpha * normal).normalize();
	const CRTVector B = C * sinBeta;
	const float cosBeta = sqrtf(1 - sinBeta * sinBeta);
	const CRTVector A = cosBeta * -1 * normal;
	const CRTVector Rdirection = A + B;
	// Construct refractionRay
	CRTRay refractionRay(bestIntersectionPoint.p, Rdirection, 0, RayType::refractive);
	// Trace refractionRay
	// ....
	return bestIntersectionPoint.material.albedo;
}

CRTColor CRTRenderer::shadeReflective(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {
	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

	// Create reflectionRay
	CRTRay reflectionRay(bestIntersectionPoint.p + shadeNormal * SHADOW_BIAS,
		ray.direction - 2 * ray.direction.dot(shadeNormal) * shadeNormal, ray.pathDepth + 1, RayType::reflection);
	CRTColor reflectionColor = intersectScene(reflectionRay);
	return reflectionColor * material.albedo;
}

CRTColor CRTRenderer::shadeDiffuse(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {

	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

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
		CRTRay shadowRay(bestIntersectionPoint.p + shadeNormal * SHADOW_BIAS, normLightDir, MAX_DEPTH, RayType::shadow);
		//Trace shadowRay to check for triangle intersection
		bool hasIntersection = hasIntersectRayWithObjectsInScene(shadowRay, scene->getGeometryObjects(), sr);
		CRTColor lightContribution = hasIntersection ? CRTColor{ } :
			CRTColor(lights[i].intensity / sa * material.albedo * cosLaw);
		finalColor += lightContribution;
	}
	return finalColor;
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
}

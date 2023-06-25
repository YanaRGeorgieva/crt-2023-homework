#include "CRTRenderer.h"

CRTColor CRTRenderer::intersectScene(const CRTRay& ray) const {
	CRTColor resultColor{};
	intersectRayWithObjectsInScene(ray, scene->getGeometryObjects(), MAX_FLOAT, resultColor);
	return resultColor;
}

CRTImage CRTRenderer::render() const {
	const int imageWidth = scene->getSettings().getImageWidth();
	const int imageHeight = scene->getSettings().getImageHeight();
	const int imageBucketSize = scene->getSettings().getImageBucketSize();

	const int numHorizontal = imageWidth / imageBucketSize;
	const int numVertical = imageHeight / imageBucketSize;

	CRTImage image(imageWidth, imageHeight);
	std::vector<CRTImage> subImages;

	for (size_t i = 0; i < numVertical; i++) {
		for (size_t j = 0; j < numHorizontal; j++) {
			// Calculate the top-left and bottom-right pixel coordinates for this sub-image
			size_t top = i * imageBucketSize;
			size_t realHeight = imageBucketSize;
			if (top + imageBucketSize > imageHeight) {
				realHeight = imageHeight - top;
			}
			size_t left = j * imageBucketSize;
			size_t realWidth = imageBucketSize;
			if (left + imageBucketSize > imageWidth) {
				realWidth = imageWidth - left;
			}
			CRTImage subImage(realWidth, realHeight, left, top);
			subImages.emplace_back(subImage);
		}
	}

	std::for_each(
		std::execution::par,
		subImages.begin(),
		subImages.end(),
		[&](CRTImage& bucket) {
			processSubimage(bucket);
		}
	);

	const int length = numVertical * numHorizontal;
	//#pragma omp parallel for
	//	for (int i = 0; i < length; i++) {
	//		processSubimage(subImages[i]);
	//	}


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

	//CRTImage image(imageWidth, imageHeight);
	//processSubimage(image);
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
			if (cIdx == 1146 && rIdx == 726) {
				int b = 0;
			}
			CRTRay cameraRay = scene->getCamera().generateCameraRay(rIdx, cIdx);
			subImage.setPixel(rowIdx, colIdx, intersectScene(cameraRay));
		}
	}
}

bool CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray,
	const std::vector<CRTMesh>& geometryObjects,
	const float& bestTDefault,
	CRTColor& col) const {

	CRTIntersectionData intersectionPoint{};
	CRTIntersectionData bestIntersectionPointInfo{};
	bestIntersectionPointInfo.isValid = false;

	float bestT{ bestTDefault };
	//float bestTBox{ maxFloat };

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		const CRTBox& aabbBox = geometryObjects[i].getBox();
		if (aabbBox.isValid()) {
			const retIntersectionBox& data = aabbBox.intersect(ray, bestT);
			if (data.isValid) {
				// Do the intersection with the object
				//bestTBox = data.t;
			} else {
				// Skip the intersection with the object
				continue;
			}
		}
		intersectionPoint = geometryObjects[i].intersect(ray, i, bestT);
		if (intersectionPoint.isValid) {
			if (ray.type == RayType::shadow) {
				return true;
			}
			bestIntersectionPointInfo = intersectionPoint;
		}
	}
	col = shade(ray, bestIntersectionPointInfo);
	return bestIntersectionPointInfo.isValid;
}

CRTColor CRTRenderer::shade(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {

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
	if (ray.pathDepth >= MAX_DEPTH) {
		return scene->getSettings().getBGColor();
	}

	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

	CRTVector normal = shadeNormal;
	float eta1{ currentIOR };
	float eta2{ material.ior };
	float angleBetweenIandN = ray.direction.dot(normal);
	// Check if the incident ray leaves the transparent object:
	if (greaterThan(angleBetweenIandN, 0.0f)) {
		// Then we need N = -N and swap(eta1, eta2):
		normal = -1.0f * normal;
		float tmp = eta1;
		eta1 = eta2;
		eta2 = tmp;
	}
	// Compute cosine between I and N (cos(180-alpha) = -cos(alpha)):
	const float cosAlpha = -1.0f * ray.direction.dot(normal);

	// If angle(I, N) <  critical angle (sin(I, N) < eta1 / eta2);
	const float sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);
	if (lessThan(sinAlpha, eta2 / eta1)) {
		// Using Snell's Law find sin(R, -N):
		const float sinBeta = (sinAlpha * eta1) / eta2;
		// Compute vector R using vector addition:
		const CRTVector C = (ray.direction + cosAlpha * normal).normalize();
		const CRTVector B = C * sinBeta;
		const float cosBeta = sqrtf(1.0f - sinBeta * sinBeta);
		const CRTVector A = cosBeta * -1.0f * normal;
		const CRTVector Rdirection = A + B;
		// Construct refractionRay
		CRTRay refractionRay(bestIntersectionPoint.p + (-1.0f * normal * REFRACTION_BIAS),
			Rdirection,
			ray.pathDepth + 1,
			RayType::refractive);
		// Trace refractionRay
		CRTColor refractionColor = intersectScene(refractionRay);
		// Construct reflectionRay
		CRTRay reflectionRay(bestIntersectionPoint.p + normal * REFLECTION_BIAS,
			ray.direction - 2.0f * ray.direction.dot(normal) * normal,
			ray.pathDepth + 1,
			RayType::reflective);
		// Trace reflectionRay
		CRTColor reflectionColor = intersectScene(reflectionRay);
		// Combine reflection and refraction color
		float fresnel = 0.5f * powf(1.0f + ray.direction.dot(normal), 5.0f);
		return fresnel * reflectionColor + (1.0f - fresnel) * refractionColor;
	} else {
		// Construct reflectionRay
		CRTRay reflectionRay(bestIntersectionPoint.p + normal * REFLECTION_BIAS,
			ray.direction - 2.0f * ray.direction.dot(normal) * normal,
			ray.pathDepth + 1,
			RayType::reflective);
		// Trace reflectionRay and return
		return intersectScene(reflectionRay);
	}
}

CRTColor CRTRenderer::shadeReflective(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint) const {
	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

	// Create reflectionRay
	CRTRay reflectionRay(bestIntersectionPoint.p + shadeNormal * REFLECTION_BIAS,
		ray.direction - 2.0f * ray.direction.dot(shadeNormal) * shadeNormal,
		ray.pathDepth + 1,
		RayType::reflective);
	CRTColor reflectionColor = intersectScene(reflectionRay);
	return reflectionColor * material.albedo;
}

CRTColor CRTRenderer::shadeDiffuse(const CRTRay& ray,
	const CRTIntersectionData& bestIntersectionPoint,
	const bool hasShadow) const {

	const CRTMaterial& material = bestIntersectionPoint.material;

	CRTVector shadeNormal = bestIntersectionPoint.triangleFace;
	if (material.isSmoothShaded) {
		shadeNormal = bestIntersectionPoint.interpolatedVerticesNormal;
	}

	CRTColor finalColor{};
	const std::vector<CRTLight>& lights = scene->getLights();
	const size_t lightsCnt = lights.size();

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
		if (lessThan(cosLaw, 0.0f)) { continue; }
		// Compute sphere area
		float sa = 4.0f * (float)M_PI * sr * sr;
		// Create shadowRay
		CRTRay shadowRay(bestIntersectionPoint.p + shadeNormal * SHADOW_BIAS,
			normLightDir, MAX_DEPTH, RayType::shadow);
		// Trace shadowRay to check for triangle intersection
		if (hasShadow) {
			CRTColor dummy{};
			bool isNotVisible = intersectRayWithObjectsInScene(shadowRay, scene->getGeometryObjects(), sr, dummy);
			finalColor += isNotVisible ? CRTColor{ 0.0f } :
				CRTColor(lights[i].intensity / sa * material.albedo * cosLaw);
		} else {
			finalColor += CRTColor(lights[i].intensity / sa * material.albedo * cosLaw);
		}
	}
	return finalColor;
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
	currentIOR = 1.0f;
}

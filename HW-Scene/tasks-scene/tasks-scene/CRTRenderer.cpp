#include "CRTRenderer.h"

CRTColor CRTRenderer::intersectScene(const int rowIdx, const int colIdx) const {
	// Generate camera ray R : 3rd Lecture taking into account the camera position
	CRTRay cameraRay = scene->getCamera().generateCameraRay(rowIdx, colIdx);
	return intersectRayWithObjectsInScene(cameraRay, scene->getGeometryObjects());
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
	const int length = numVertical * numHorizontal;

#pragma omp parallel
#pragma omp for
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
			subImage.setPixel(rowIdx, colIdx, intersectScene(top + rowIdx, left + colIdx));
		}
	}
}

CRTColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const {
	CRTVector bestP{ worstP };
	// When we have many overlapping triangles
	std::vector < OverlappingTriangles >partialOverlappingTriangles;

	const CRTRGBColor& bgCol = scene->getSettings().getBGColor();
	CRTColor returnCol;
	returnCol.setComponents((float)bgCol.r / (float)maxRGBColorComponent,
							(float)bgCol.g / (float)maxRGBColorComponent,
							(float)bgCol.b / (float)maxRGBColorComponent);

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectRayWithAnObject(ray, i, geometryObjects[i], bestP, partialOverlappingTriangles);
	}

	if (!partialOverlappingTriangles.empty()) {
		const size_t lenTri = partialOverlappingTriangles.size();
		CRTColor col{};
		for (size_t i = 0; i < lenTri; i++) {
			col += partialOverlappingTriangles[i].color;
		}
		col = col / (int)lenTri;
		returnCol = col;
	}

	return returnCol;
}

void CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
										   const size_t idxGeometryObject,
										   const CRTMesh& geometryObject,
										   CRTVector& currentBestP,
										   std::vector < OverlappingTriangles>& partialOverlappingTriangles) const {
	CRTVector bestP{ currentBestP };

	const std::vector<CRTTriangle>& triangles = geometryObject.getTriangles();
	const std::vector<CRTVector>& colors = geometryObject.getColors();

	const size_t len = triangles.size();
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const CRTVector& p = tri.intersect(ray);
		//	If P is on the left of the 3 edges, we have an intersection
		if (bestP.length() > p.length()) {
			// Get closest p (with least length)
			bestP = p;
			partialOverlappingTriangles.clear();
			partialOverlappingTriangles.push_back({ idxGeometryObject, i, colors[i] });
		} else if (!equals(worstP.length(), p.length()) && equals(bestP.length(), p.length())) {
			partialOverlappingTriangles.push_back({ idxGeometryObject, i, colors[i] });
		}
	}

	if (bestP.length() < currentBestP.length()) {
		currentBestP = bestP;
	}
}

void CRTRenderer::loadCRTScene(const std::string& sceneFilename) {
	scene->parseSceneFile(sceneFilename);
}

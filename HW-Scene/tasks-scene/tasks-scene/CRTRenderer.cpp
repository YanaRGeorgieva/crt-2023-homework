#include "CRTRenderer.h"

void CRTRenderer::renderInPPMFile(const std::string& filename) {
	const int imageWidth = scene->getSettings().getImageWidth();
	const int imageHeight = scene->getSettings().getImageHeight();

	std::ofstream ppmFileStream(std::string(filename) + ".ppm", std::ios::out | std::ios::binary);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << std::endl;
	ppmFileStream << maxColorComponent << std::endl;

	RGBColor returnColor{};
	// Generate triangles normal vectors : normalize(cross(E0, E1)) - precomputed 
	// At each pixel :
	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
			returnColor = RGBColor{ 0, 0, 0 };
			// Generate camera ray R : 3rd Lecture taking into account the camera position
			CRTRay cameraRay = scene->getCamera().generateCameraRay(rowIdx, colIdx);
			returnColor = intersectRayWithObjectsInScene(cameraRay, scene->getGeometryObjects());
			ppmFileStream << (int)returnColor.r << " " << (int)returnColor.g << " " << (int)returnColor.b << "\t";
		}
		ppmFileStream << std::endl;
	}
	ppmFileStream.close();
}

RGBColor CRTRenderer::intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) {
	CRTVector bestP{ worstP };
	// When we have many overlapping triangles
	std::vector < OverlappingTriangles >partialOverlappingTriangles;

	RGBColor returnCol(scene->getSettings().getBGColor());

	const size_t len = geometryObjects.size();
	for (size_t i = 0; i < len; i++) {
		intersectRayWithAnObject(ray, i, geometryObjects[i], bestP, partialOverlappingTriangles);
	}

	if (!partialOverlappingTriangles.empty()) {
		const size_t lenTri = partialOverlappingTriangles.size();
		CRTVector col{};
		for (size_t i = 0; i < lenTri; i++) {
			col += partialOverlappingTriangles[i].color;
		}
		col = col / (int)lenTri;
		returnCol = RGBColor(col);
	}

	return returnCol;
}

void CRTRenderer::intersectRayWithAnObject(const CRTRay& ray,
										   const size_t idxGeometryObject,
										   const CRTMesh& geometryObject,
										   CRTVector& currentBestP,
										   std::vector < OverlappingTriangles>& partialOverlappingTriangles) {
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

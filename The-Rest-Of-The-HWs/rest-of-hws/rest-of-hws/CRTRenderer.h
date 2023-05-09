#ifndef CRTRENDERER_H
#define CRTRENDERER_H

#include <vector>
#include <string>

#include <omp.h>

#include "CRTRay.h"
#include "CRTMesh.h"
#include "CRTColor.h"
#include "CRTImage.h"
#include "CRTScene.h"
#include "constants.h"
#include "CRTMaterial.h"
#include "CRTTriangle.h"
#include "CRTRGBColor.h"

class CRTRenderer {
public:
	friend class CRTScene;
	CRTRenderer() {
		scene = std::unique_ptr<CRTScene>(new CRTScene);
	}

	CRTRenderer(const CRTRenderer&) = delete;
	CRTRenderer& operator=(const CRTRenderer&) = delete;

	CRTImage render(const int maxNumberOfBuckets = 64) const;
	CRTColor intersectScene(const CRTRay& ray) const;

	void loadCRTScene(const std::string& sceneFilename);
	//bool exportCRTScene(const std::string& sceneFilename) const;

private:
	struct InformationIntersectionPoint {
		size_t idxGeometry;
		size_t idxTriangle;
		CRTVector p;
		CRTVector triN;
		CRTColor hitNormal;
		CRTColor barycentricCoordinates;
		bool isValid;
		size_t materialIndex;

		void set(const size_t& idxGeometryObject,
			const size_t& idxTriangleI,
			const CRTTriangle& triangle,
			const CRTTriangle::retDataFromTriIntersect intersectData,
			const CRTMesh& geometryObject,
			const std::vector<CRTVector>& vertexNormals,
			const std::vector<size_t>& faces
		) {
			idxGeometry = idxGeometryObject;
			idxTriangle = idxTriangleI;
			p = intersectData.p;
			triN = triangle.getFaceNormal();
			const size_t idx = idxTriangleI * 3;
			hitNormal =
				(vertexNormals[faces[idx + 1]] * intersectData.barycentricCoordinates.x +
					vertexNormals[faces[idx + 2]] * intersectData.barycentricCoordinates.y +
					vertexNormals[faces[idx]] * intersectData.barycentricCoordinates.z).normalize();
			barycentricCoordinates = intersectData.barycentricCoordinates;
			isValid = true;
			materialIndex = geometryObject.getMaterialIdx();
		}
	};

	InformationIntersectionPoint intersectRayWithAnObject(const CRTRay& ray, const size_t idxGeometryObject, const CRTMesh& geometryObject) const;
	CRTColor intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects, int depth = 0) const;
	bool hasIntersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects, const float lightLength) const;

	CRTColor shade(const InformationIntersectionPoint& bestIntersectionPoint,
		const CRTVector& shadeNormal,
		const CRTMaterial& material) const;

	void processSubimage(CRTImage& subImage) const;

	std::unique_ptr<CRTScene> scene;
};

#endif // !CRTRENDERER_H
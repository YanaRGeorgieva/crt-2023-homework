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
#include "CRTIntersectionData.h"

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

	CRTIntersectionData intersectRayWithAnObject(const CRTRay& ray,
		const size_t idxGeometryObject,
		const CRTMesh& geometryObject,
		float& bestT) const;

	CRTColor intersectRayWithObjectsInScene(const CRTRay& ray,
		const std::vector<CRTMesh>& geometryObjects) const;

	bool hasIntersectRayWithObjectsInScene(const CRTRay& ray,
		const std::vector<CRTMesh>& geometryObjects,
		const float thresholdPminusLight) const;

	CRTColor shadeRefractive(const CRTRay& ray,
		const CRTIntersectionData& bestIntersectionPoint) const;

	CRTColor shadeReflective(const CRTRay& ray,
		const CRTIntersectionData& bestIntersectionPoint) const;

	CRTColor shadeConstant(const CRTRay& ray,
		const CRTIntersectionData& bestIntersectionPoint) const;

	CRTColor shadeDiffuse(const CRTRay& ray,
		const CRTIntersectionData& bestIntersectionPoint) const;

	CRTColor shade(const CRTRay & ray, 
		const CRTIntersectionData& bestIntersectionPoint) const;

	void processSubimage(CRTImage& subImage) const;

	std::unique_ptr<CRTScene> scene;
};

#endif // !CRTRENDERER_H
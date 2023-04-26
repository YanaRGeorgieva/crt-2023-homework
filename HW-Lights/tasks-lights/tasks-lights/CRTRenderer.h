#ifndef CRTRENDERER_H
#define CRTRENDERER_H

#include <vector>
#include <string>

#include "CRTRay.h"
#include "CRTMesh.h"
#include "CRTColor.h"
#include "CRTImage.h"
#include "CRTScene.h"
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

	void setShadowBias(const float shadowBais);
	void setAlbedo(const CRTColor& albedo);

	void loadCRTScene(const std::string& sceneFilename);
	//bool exportCRTScene(const std::string& sceneFilename) const;

private:
	struct OverlappingTriangles {
		size_t idxGeometry;
		size_t idxTriangle;
		CRTVector p;
		CRTVector triN;
		CRTColor color;
	};
	void intersectRayWithAnObject(const CRTRay& ray,
								  const size_t idxGeometryObject,
								  const CRTMesh& geometryObject,
								  CRTVector& currentBestP,
								  std::vector <OverlappingTriangles >& partialOverlappingTriangles) const;
	bool hasIntersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const;
	CRTColor intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects) const;
	void processSubimage(CRTImage& subImage) const;

	std::unique_ptr<CRTScene> scene;
	CRTVector albedo;
	float shadowBias;
};

#endif // !CRTRENDERER_H
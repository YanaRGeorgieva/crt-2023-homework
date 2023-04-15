#ifndef CRTRENDERER_H
#define CRTRENDERER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "CRTVector.h"
#include "CRTRay.h"
#include "CRTMesh.h"
#include "CRTScene.h"
#include "CRTTriangle.h"

class CRTRenderer {
public:
	CRTRenderer() {
		scene = new CRTScene;
	}

	~CRTRenderer() {
		delete scene;
	}

	CRTRenderer(const CRTRenderer&) = delete;
	CRTRenderer& operator=(const CRTRenderer&) = delete;

	void renderInPPMFile(const std::string& filename);

	void loadCRTScene(const std::string& sceneFilename);
	//bool exportCRTScene(const std::string& sceneFilename) const;

private:
	CRTScene* scene = NULL;

	void intersectRayWithAnObject(const CRTRay& ray,
								  const size_t idxGeometryObject,
								  const CRTMesh& geometryObject,
								  CRTVector& currentBestP,
								  std::vector <OverlappingTriangles >& partialOverlappingTriangles);
	RGBColor intersectRayWithObjectsInScene(const CRTRay& ray, const std::vector<CRTMesh>& geometryObjects);
};

#endif // !CRTRENDERER_H
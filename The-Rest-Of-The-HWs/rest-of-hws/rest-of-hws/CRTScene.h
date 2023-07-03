#ifndef CRTSCENE_H
#define CRTSCENE_H

#include <string>
#include <vector>

#include "CRTMesh.h"
#include "CRTLight.h"
#include "constants.h"
#include "CRTCamera.h"
#include "CRTParser.h"
#include "CRTMaterial.h"
#include "CRTSettings.h"

class CRTScene {
public:
	CRTScene() { }

	~CRTScene() {
		geometryObjects.clear();
		lights.clear();
	}

	CRTScene(const CRTScene&) = delete;
	const CRTScene& operator=(const CRTScene&) = delete;

	const std::vector<CRTMesh>& getGeometryObjects() const;
	CRTCamera& getCamera();
	const CRTSettings& getSettings() const;
	const std::vector<CRTLight>& getLights() const;
	const std::vector<CRTMaterial>& getMaterials() const;

	void parseSceneFile(const std::string& sceneFileName);

private:
	std::vector<CRTMesh> geometryObjects;
	CRTCamera camera;
	CRTSettings settings;
	std::vector<CRTLight> lights;
	std::vector<CRTMaterial> materials;

	CRTParser parser;
};

#endif // !CRTSCENE_H
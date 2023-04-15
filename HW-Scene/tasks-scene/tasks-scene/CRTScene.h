#ifndef CRTSCENE_H
#define CRTSCENE_H

#include <string>
#include <vector>

#include "CRTMesh.h"
#include "CRTCamera.h"
#include "CRTSettings.h"
#include "CRTMesh.h"
#include "CRTParser.h"

class CRTScene {
public:
	CRTScene() { }

	~CRTScene() {
		geometryObjects.clear();
	}

	CRTScene(const CRTScene&) = delete;
	const CRTScene& operator=(const CRTScene&) = delete;

	std::vector<CRTMesh>& getGeometryObjects();
	CRTCamera& getCamera();
	CRTSettings& getSettings();

	const std::vector<CRTMesh>& getGeometryObjects()const;
	const CRTCamera& getCamera()const;
	const CRTSettings& getSettings()const;

	void parseSceneFile(const std::string& sceneFileName);

private:
	std::vector<CRTMesh> geometryObjects;
	CRTCamera camera;
	CRTSettings settings;

	CRTParser parser;
};

#endif // !CRTSCENE_H
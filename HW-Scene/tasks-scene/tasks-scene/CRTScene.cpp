#include "CRTScene.h"

const std::vector<CRTMesh>& CRTScene::getGeometryObjects() const {
	return geometryObjects;
}

const CRTCamera& CRTScene::getCamera() const {
	return camera;
}

const CRTSettings& CRTScene::getSettings() const {
	return settings;
}

void CRTScene::parseSceneFile(const std::string& sceneFileName) {
	parser.loadJsonDocument(sceneFileName);
	parser.loadSettingsAndCamera(camera, settings);
	geometryObjects = parser.loadObjects();
}
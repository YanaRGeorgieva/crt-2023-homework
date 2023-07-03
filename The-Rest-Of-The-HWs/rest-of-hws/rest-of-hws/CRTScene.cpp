#include "CRTScene.h"

const std::vector<CRTMesh>& CRTScene::getGeometryObjects() const {
	return geometryObjects;
}

CRTCamera& CRTScene::getCamera() {
	return camera;
}

const CRTSettings& CRTScene::getSettings() const {
	return settings;
}

const std::vector<CRTLight>& CRTScene::getLights() const {
	return lights;
}

const std::vector<CRTMaterial>& CRTScene::getMaterials() const {
	return materials;
}

void CRTScene::parseSceneFile(const std::string& sceneFileName) {
	parser.loadJsonDocument(sceneFileName);
	parser.loadSettingsAndCamera(camera, settings);
	materials = parser.loadMaterials();
	lights = parser.loadLights();
	geometryObjects = parser.loadObjects();
	for (auto& object : geometryObjects) {
		object.generateGeometry();
		object.setMaterial(materials[object.getMaterialIdx()]);
	}
}
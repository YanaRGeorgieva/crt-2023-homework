#ifndef CRTPARSER_H
#define CRTPARSER_H

#include <vector>
#include <fstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "CRTMesh.h"
#include "CRTLight.h"
#include "CRTCamera.h"
#include "CRTMatrix.h"
#include "CRTVector.h"
#include "CRTSettings.h"

static const char* crtSceneSettings = "settings";
static const char* crtSceneBGColor = "background_color";
static const char* crtSceneImageSettings = "image_settings";
static const char* crtSceneImageWidth = "width";
static const char* crtSceneImageHeight = "height";
static const char* crtSceneCamera = "camera";
static const char* crtSceneMatrix = "matrix";
static const char* crtScenePosition = "position";
static const char* crtSceneObjects = "objects";
static const char* crtSceneVertices = "vertices";
static const char* crtSceneTriangles = "triangles";
static const char* crtSceneLights = "lights";
static const char* crtSceneLightsPosition = "position";
static const char* crtSceneLightsIntensity = "intensity";

class CRTParser {
public:
	CRTParser() {
		crtDoc =  std::unique_ptr<rapidjson::Document>(new rapidjson::Document);
	}

	CRTParser(const CRTParser&) = delete;
	const CRTParser& operator=(const CRTParser&) = delete;

	void loadJsonDocument(const std::string& filename);
	void loadSettingsAndCamera(CRTCamera& camera, CRTSettings& settings) const;
	std::vector<CRTMesh> loadObjects() const;
	std::vector<CRTLight> loadLights() const;

private:
	void loadVector(const rapidjson::Value::ConstArray& arr, CRTVector& vec) const;
	CRTVector loadVector(const rapidjson::Value::ConstArray& arr) const;
	void loadMatrix(const rapidjson::Value::ConstArray& arr, CRTMatrix& mat) const;
	CRTMatrix loadMatrix(const rapidjson::Value::ConstArray& arr) const;
	std::vector<CRTVector> loadArrayOfVectors(const rapidjson::Value::ConstArray& arr) const;
	std::vector<size_t> loadArrayOfUInts(const rapidjson::Value::ConstArray& arr) const;

	std::unique_ptr<rapidjson::Document> crtDoc;
};

#endif // !CRTPARSER_H
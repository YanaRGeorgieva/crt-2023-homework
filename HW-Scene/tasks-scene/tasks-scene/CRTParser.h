#ifndef CRTPARSER_H
#define CRTPARSER_H

#include <fstream>
#include <iostream>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "CRTVector.h"
#include "CRTMatrix.h"
#include "CRTSettings.h"
#include "CRTCamera.h"
#include "CRTMesh.h"

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

class CRTParser {
public:
	CRTParser() {
		crtDoc = new rapidjson::Document;
	}

	CRTParser(const CRTParser&) = delete;
	const CRTParser& operator=(const CRTParser&) = delete;

	~CRTParser() {
		delete crtDoc;
	}

	void loadJsonDocument(const std::string& filename);
	void loadSettingsAndCamera(CRTCamera& camera, CRTSettings& settings) const;
	std::vector<CRTMesh> loadObjects() const;

private:
	void loadVector(const rapidjson::Value::ConstArray& arr, CRTVector& vec) const;
	void loadMatrix(const rapidjson::Value::ConstArray& arr, CRTMatrix& mat) const;
	std::vector<CRTVector>  loadArrayOfVectors(const rapidjson::Value::ConstArray& arr) const;
	std::vector<size_t> loadArrayOfUInts(const rapidjson::Value::ConstArray& arr) const;

	rapidjson::Document* crtDoc;
};

#endif // !CRTPARSER_H
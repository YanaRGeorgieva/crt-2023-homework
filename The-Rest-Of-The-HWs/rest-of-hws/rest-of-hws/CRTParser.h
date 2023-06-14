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
#include "CRTMaterial.h"
#include "CRTSettings.h"

static const char* crtSceneSettings = "settings";
static const char* crtSceneBGColor = "background_color";
static const char* crtSceneImageSettings = "image_settings";
static const char* crtSceneImageWidth = "width";
static const char* crtSceneImageHeight = "height";
static const char* crtSceneBucketSize = "bucket_size";

static const char* crtSceneCamera = "camera";
static const char* crtSceneCameraMatrix = "matrix";
static const char* crtSceneCameraPosition = "position";

static const char* crtSceneObjects = "objects";
static const char* crtSceneObjectVertices = "vertices";
static const char* crtSceneObjectTriangles = "triangles";
static const char* crtSceneObjectMaterialIndex = "material_index";

static const char* crtSceneLights = "lights";
static const char* crtSceneLightsPosition = "position";
static const char* crtSceneLightsIntensity = "intensity";

static const char* crtSceneMaterials = "materials";
static const char* crtSceneMaterialType = "type";
static const char* crtSceneMaterialAlbedo = "albedo";
static const char* crtSceneMaterialIor = "ior";
static const char* crtSceneMaterialIsSmoothShaded = "smooth_shading";

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
	std::vector<CRTMaterial> loadMaterials() const;

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
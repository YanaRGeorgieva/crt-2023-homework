#include "CRTParser.h"

void CRTParser::loadJsonDocument(const std::string& filename) {
	std::ifstream ifs(filename);
	assert(ifs.is_open());

	rapidjson::IStreamWrapper isw(ifs);
	crtDoc->ParseStream(isw);

	if (crtDoc->HasParseError()) {
		std::cerr << "Error  :" << crtDoc->GetParseError() << '\n';
		std::cerr << "Offset :" << crtDoc->GetErrorOffset() << '\n';
		assert(false);
	}

	assert(crtDoc->IsObject());
}

void CRTParser::loadVector(const rapidjson::Value::ConstArray& arr, CRTVector& vec) const {
	assert(arr.Size() == 3);
	vec = CRTVector{ static_cast<float>(arr[0].GetDouble()),
		static_cast<float>(arr[1].GetDouble()),
		static_cast<float>(arr[2].GetDouble()) };
}

CRTVector CRTParser::loadVector(const rapidjson::Value::ConstArray& arr) const {
	assert(arr.Size() == 3);
	return CRTVector{ static_cast<float>(arr[0].GetDouble()),
		static_cast<float>(arr[1].GetDouble()),
		static_cast<float>(arr[2].GetDouble()) };
}

void CRTParser::loadMatrix(const rapidjson::Value::ConstArray& arr, CRTMatrix& mat) const {
	assert(arr.Size() == 9);
	mat = CRTMatrix{
				CRTVector{static_cast<float>(arr[0].GetDouble()),
			static_cast<float>(arr[1].GetDouble()),
			static_cast<float>(arr[2].GetDouble())},
				CRTVector{ static_cast<float>(arr[3].GetDouble()),
			static_cast<float>(arr[4].GetDouble()),
			static_cast<float>(arr[5].GetDouble())},
				CRTVector{ static_cast<float>(arr[6].GetDouble()),
			static_cast<float>(arr[7].GetDouble()),
			static_cast<float>(arr[8].GetDouble())}
	};
}

CRTMatrix CRTParser::loadMatrix(const rapidjson::Value::ConstArray& arr) const {
	assert(arr.Size() == 9);
	return CRTMatrix{
				CRTVector{static_cast<float>(arr[0].GetDouble()),
			static_cast<float>(arr[1].GetDouble()),
			static_cast<float>(arr[2].GetDouble())},
				CRTVector{ static_cast<float>(arr[3].GetDouble()),
			static_cast<float>(arr[4].GetDouble()),
			static_cast<float>(arr[5].GetDouble())},
				CRTVector{ static_cast<float>(arr[6].GetDouble()),
			static_cast<float>(arr[7].GetDouble()),
			static_cast<float>(arr[8].GetDouble())}
	};
}

std::vector<CRTVector> CRTParser::loadArrayOfVectors(const rapidjson::Value::ConstArray& arr) const {
	const rapidjson::SizeType len = arr.Size();
	assert(len % 3 == 0);
	std::vector<CRTVector> vertices;
	vertices.reserve(len / 3);
	for (rapidjson::SizeType i = 0; i < len; i += 3) {
		vertices.emplace_back(CRTVector{ static_cast<float>(arr[i].GetDouble()),
						static_cast<float>(arr[i + 1].GetDouble()),
						static_cast<float>(arr[i + 2].GetDouble()) }
		);
	}
	return vertices;
}

std::vector<size_t> CRTParser::loadArrayOfUInts(const rapidjson::Value::ConstArray& arr) const {
	const rapidjson::SizeType len = arr.Size();
	assert(len % 3 == 0);
	std::vector<size_t> faces;
	faces.reserve(len);
	for (rapidjson::SizeType i = 0; i < len; i++) {
		faces.emplace_back(static_cast<size_t>(arr[i].GetInt()));
	}
	return faces;
}

void CRTParser::loadSettingsAndCamera(CRTCamera& camera, CRTSettings& settings) const {
	const rapidjson::Value& settingsVal = crtDoc->FindMember(crtSceneSettings)->value;
	if (!settingsVal.IsNull() && settingsVal.IsObject()) {
		const rapidjson::Value& bgColorVal = settingsVal.FindMember(crtSceneBGColor)->value;
		assert(!bgColorVal.IsNull() && bgColorVal.IsArray());
		loadVector(bgColorVal.GetArray(), settings.bgColor);

		const rapidjson::Value& imageSettingsVal = settingsVal.FindMember(crtSceneImageSettings)->value;
		if (!imageSettingsVal.IsNull() && imageSettingsVal.IsObject()) {
			const rapidjson::Value& imageWidthVal = imageSettingsVal.FindMember(crtSceneImageWidth)->value;
			const rapidjson::Value& imageHeightVal = imageSettingsVal.FindMember(crtSceneImageHeight)->value;
			const rapidjson::Value& imageBucketSizeVal = imageSettingsVal.FindMember(crtSceneBucketSize)->value;
			assert(!imageWidthVal.IsNull() && imageWidthVal.IsInt()
				&& !imageHeightVal.IsNull() && imageHeightVal.IsInt());
			if (!imageBucketSizeVal.IsNull() && imageBucketSizeVal.IsInt()) {
				settings.fillImageSettings(imageWidthVal.GetInt(), imageHeightVal.GetInt(), imageBucketSizeVal.GetInt());
			} else {
				settings.fillImageSettings(imageWidthVal.GetInt(), imageHeightVal.GetInt());
			}
			camera.updateImageSettings(settings.imageSettings);
		}
	}

	const rapidjson::Value& cameraVal = crtDoc->FindMember(crtSceneCamera)->value;
	if (!cameraVal.IsNull() && cameraVal.IsObject()) {
		const rapidjson::Value& matrixVal = cameraVal.FindMember(crtSceneCameraMatrix)->value;
		assert(!matrixVal.IsNull() && matrixVal.IsArray());
		loadMatrix(matrixVal.GetArray(), camera.rotation);

		const rapidjson::Value& positionVal = cameraVal.FindMember(crtSceneCameraPosition)->value;
		assert(!positionVal.IsNull() && positionVal.IsArray());
		loadVector(positionVal.GetArray(), camera.position);
	}
}

std::vector<CRTMesh> CRTParser::loadObjects() const {
	std::vector<CRTMesh> geometryObjects;
	const rapidjson::Value& objectsVal = crtDoc->FindMember(crtSceneObjects)->value;
	if (!objectsVal.IsNull() && objectsVal.IsArray()) {
		auto arr = objectsVal.GetArray();
		rapidjson::SizeType len = arr.Size();
		geometryObjects.reserve(arr.Size());
		for (rapidjson::SizeType i = 0; i < len; i++) {
			const rapidjson::Value& verticesVal = arr[i][crtSceneObjectVertices];
			assert(!verticesVal.IsNull() && verticesVal.IsArray());

			const rapidjson::Value& trianglesVal = arr[i][crtSceneObjectTriangles];
			assert(!trianglesVal.IsNull() && trianglesVal.IsArray());

			const rapidjson::Value& materialIndexVal = arr[i][crtSceneObjectMaterialIndex];
			assert(!materialIndexVal.IsNull() && materialIndexVal.IsUint());

			geometryObjects.emplace_back(
				loadArrayOfVectors(verticesVal.GetArray()), 
				loadArrayOfUInts(trianglesVal.GetArray()), 
				materialIndexVal.GetUint());
		}
	}
	return geometryObjects;
}

std::vector<CRTLight> CRTParser::loadLights() const {
	std::vector<CRTLight> lights;
	const rapidjson::Value& lightsVal = crtDoc->FindMember(crtSceneLights)->value;
	if (!lightsVal.IsNull() && lightsVal.IsArray()) {
		auto arr = lightsVal.GetArray();
		rapidjson::SizeType len = arr.Size();
		lights.reserve(arr.Size());
		for (rapidjson::SizeType i = 0; i < len; i++) {
			const rapidjson::Value& lightPositionVal = arr[i][crtSceneLightsPosition];
			assert(!lightPositionVal.IsNull() && lightPositionVal.IsArray());

			const rapidjson::Value& lightIntensityVal = arr[i][crtSceneLightsIntensity];
			assert(!lightIntensityVal.IsNull() && lightIntensityVal.IsUint());

			lights.emplace_back(
				loadVector(lightPositionVal.GetArray()), 
				static_cast<float>(lightIntensityVal.GetDouble()));
		}
	}
	return lights;
}

std::vector<CRTMaterial> CRTParser::loadMaterials() const {
	std::vector<CRTMaterial> materials;
	const rapidjson::Value& materialsVal = crtDoc->FindMember(crtSceneMaterials)->value;
	if (!materialsVal.IsNull() && materialsVal.IsArray()) {
		auto arr = materialsVal.GetArray();
		rapidjson::SizeType len = arr.Size();
		materials.reserve(arr.Size());
		for (rapidjson::SizeType i = 0; i < len; i++) {
			const rapidjson::Value& materialTypeVal = arr[i][crtSceneMaterialType];
			assert(!materialTypeVal.IsNull() && materialTypeVal.IsString());

			const rapidjson::Value* materialAlbedoVal = NULL;
			const rapidjson::Value::ConstMemberIterator materialAlbedoItr = arr[i].FindMember(crtSceneMaterialAlbedo);
			if (materialAlbedoItr != arr[i].MemberEnd()) {
				materialAlbedoVal = &arr[i][crtSceneMaterialAlbedo];
				assert(!materialAlbedoVal->IsNull() && materialAlbedoVal->IsArray());
			}

			const rapidjson::Value* materialIorVal = NULL;
			const rapidjson::Value::ConstMemberIterator materialIorItr = arr[i].FindMember(crtSceneMaterialIor);
			if (materialIorItr != arr[i].MemberEnd()) {
				materialIorVal = &arr[i][crtSceneMaterialIor];
				assert(!materialIorVal->IsNull() && materialIorVal->IsDouble());
			}

			const rapidjson::Value& materialIsSmoothShaded = arr[i][crtSceneMaterialIsSmoothShaded];
			assert(!materialIsSmoothShaded.IsNull() && materialIsSmoothShaded.IsBool());

			materials.emplace_back(
				materialAlbedoVal ? loadVector(materialAlbedoVal->GetArray()) : CRTVector(1.0f), 
				materialTypeVal.GetString(), 
				materialIsSmoothShaded.GetBool(), 
				materialIorVal ? static_cast<float>(materialIorVal->GetDouble()) : 0.0f);
		}
	}
	return materials;
}
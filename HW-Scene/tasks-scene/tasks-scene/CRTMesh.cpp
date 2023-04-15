#include "CRTMesh.h"

void CRTMesh::generateGeometryAndColoring() {
	triangles.reserve(faces.size() / 3 + 1);
	colors.reserve(faces.size() / 3 + 1);
	// In interval [0,1)
	auto randomFloat = [] { return (float)(rand()) / (float)(RAND_MAX); };
	for (size_t i = 0; i < faces.size(); i += 3) {
		triangles.emplace_back(CRTTriangle{ vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]] });
		colors.emplace_back(CRTVector{ randomFloat() , randomFloat(), randomFloat() });
	}
}
const std::vector<CRTVector>& CRTMesh::getVertices() const {
	return vertices;
}

const std::vector<size_t>& CRTMesh::getFaces() const {
	return faces;
}

const std::vector<CRTTriangle>& CRTMesh::getTriangles() const {
	return triangles;
}

const std::vector<CRTVector>& CRTMesh::getColors() const {
	return colors;
}
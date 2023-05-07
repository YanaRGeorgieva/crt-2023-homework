#include "CRTMesh.h"

void CRTMesh::generateGeometry() {
	triangles.reserve(faces.size() / 3 + 1);
	vertexNormals.reserve(vertices.size());
	vertexNormals.resize(vertices.size());
	// In interval [0,1)
	size_t lenFaces = faces.size();
	for (size_t i = 0; i < lenFaces; i += 3) {
		triangles.emplace_back(CRTTriangle{ vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]] });
		vertexNormals[faces[i]] += triangles.back().getFaceNormal();
		vertexNormals[faces[i + 1]] += triangles.back().getFaceNormal();
		vertexNormals[faces[i + 2]] += triangles.back().getFaceNormal();
	}

	size_t lenVertices = vertices.size();
	for (size_t i = 0; i < vertices.size(); i++) {
		vertexNormals[i] = vertexNormals[i].normalize();
	}
}
const std::vector<CRTVector>& CRTMesh::getVertices() const {
	return vertices;
}

const std::vector<size_t>& CRTMesh::getFaces() const {
	return faces;
}

const std::vector<CRTVector>& CRTMesh::getVertexNormals() const {
	return vertexNormals;
}

const std::vector<CRTTriangle>& CRTMesh::getTriangles() const {
	return triangles;
}

const size_t& CRTMesh::getMaterialIdx() const {
	return materialIdx;
}
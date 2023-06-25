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

	calculateAABB();
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

const CRTMaterial& CRTMesh::getMaterial() const {
	return material;
}

const CRTBox& CRTMesh::getBox() const {
	return box;
}

void CRTMesh::setMaterial(const std::vector<CRTMaterial>& materials) {
	material = materials[materialIndex];
}

void CRTMesh::calculateAABB() {
	const std::vector<CRTVector>& vertices = getVertices();
	for (auto& vertex : vertices) {
		// Expand box with vertex
		box.expand(vertex);
	}
}

CRTIntersectionData CRTMesh::intersect(const CRTRay& ray,
	const size_t idxGeometryObject,
	float& bestT,
	const bool isShadowRay) const {
	CRTIntersectionData intersectionPointInfo{};
	intersectionPointInfo.isValid = false;

	const size_t len = triangles.size();
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const CRTTriangle::retDataFromTriangleIntersect& intersectData = tri.intersect(ray, bestT);
		//	If P is on the left of the 3 edges and t > 0, we have an intersection
		if (intersectData.isValid) {
			if (isShadowRay) {
				// One intersection suffices
				intersectionPointInfo.isValid = true;
				return intersectionPointInfo;
			}
			// Get closest p (with least length)
			bestT = intersectData.t;
			intersectionPointInfo.set(idxGeometryObject, i, tri, intersectData, this->material, vertexNormals, faces);
		}
	}
	return intersectionPointInfo;
}
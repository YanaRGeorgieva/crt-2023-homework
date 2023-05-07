#ifndef CRTMESH_H
#define CRTMESH_H

#include <vector>

#include "constants.h"
#include "CRTVector.h"
#include "CRTTriangle.h"

class CRTMesh {
	friend class CRTParser;
public:
	CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<size_t>& faces, const size_t& materialIdx = 0) {
		this->vertices.reserve(vertices.size());
		this->faces.reserve(faces.size());
		this->vertices = vertices;
		this->faces = faces;
		this->materialIdx = materialIdx;
	}

	void generateGeometry();
	const std::vector<CRTVector>& getVertices() const;
	const std::vector<size_t>& getFaces() const;
	const std::vector<CRTVector>& getVertexNormals() const;
	const std::vector<CRTTriangle>& getTriangles() const;
	const size_t& getMaterialIdx() const;

private:
	std::vector<CRTVector> vertices;
	std::vector<size_t> faces;

	std::vector<CRTVector> vertexNormals;
	std::vector<CRTTriangle> triangles;

	size_t materialIdx;
};

#endif // !CRTMESH_H
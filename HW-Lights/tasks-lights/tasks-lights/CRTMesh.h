#ifndef CRTMESH_H
#define CRTMESH_H

#include <ctime>
#include <vector>

#include "utils.h"
#include "CRTVector.h"
#include "CRTTriangle.h"

class CRTMesh {
	friend class CRTParser;
public:
	CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<size_t>& faces) {
		this->vertices.reserve(vertices.size());
		this->faces.reserve(faces.size());
		this->vertices = vertices;
		this->faces = faces;
	}

	void generateGeometryAndColoring();
	const std::vector<CRTVector>& getVertices() const;
	const std::vector<size_t>& getFaces() const;
	const std::vector<CRTTriangle>& getTriangles() const;
	const std::vector<CRTVector>& getColors() const;

private:
	std::vector<CRTVector> vertices;
	std::vector<size_t> faces;

	std::vector<CRTTriangle> triangles;
	std::vector<CRTVector> colors;
};

#endif // !CRTMESH_H
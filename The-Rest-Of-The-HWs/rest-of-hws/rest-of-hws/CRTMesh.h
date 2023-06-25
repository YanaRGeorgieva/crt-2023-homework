#ifndef CRTMESH_H
#define CRTMESH_H

#include <vector>

#include "CRTBox.h"
#include "constants.h"
#include "CRTVector.h"
#include "CRTMaterial.h"
#include "CRTTriangle.h"
#include "CRTIntersectionData.h"

class CRTMesh {
	friend class CRTParser;
public:
	CRTMesh(const std::vector<CRTVector>& vertices, const std::vector<size_t>& faces, const size_t& materialIndex = 0) {
		this->faces.reserve(faces.size());
		this->vertices = vertices;
		this->faces = faces;
		this->materialIndex = materialIndex;
	}

	void generateGeometry();
	const std::vector<CRTVector>& getVertices() const;
	const std::vector<size_t>& getFaces() const;
	const std::vector<CRTVector>& getVertexNormals() const;
	const std::vector<CRTTriangle>& getTriangles() const;
	const CRTMaterial& getMaterial() const;
	const CRTBox& getBox() const;
	void setMaterial(const std::vector<CRTMaterial>& materials);
	void calculateAABB();
	CRTIntersectionData intersect(const CRTRay& ray,
		const size_t idxGeometryObject,
		float& bestT,
		const bool isShadowRay = false) const;

private:
	std::vector<CRTVector> vertices;
	std::vector<size_t> faces;

	std::vector<CRTVector> vertexNormals;
	std::vector<CRTTriangle> triangles;

	size_t materialIndex;
	CRTMaterial material;

	CRTBox box;
};

#endif // !CRTMESH_H
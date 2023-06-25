#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

#include "CRTMesh.h"
#include "CRTColor.h"
#include "CRTMaterial.h"
#include "CRTTriangle.h"

struct CRTIntersectionData {
	size_t idxGeometry; ///< Closest object to the ray origin
	size_t idxTriangle; ///< Closest tringle of the closest object to the ray origin
	CRTVector p; ///< The hit point in 3D world space
	float t; ///< Distance from ray origin to the closest triangle of the closest object to the ray origin
	CRTVector triangleFace; ///< Closest tringle normal vector
	CRTColor interpolatedVerticesNormal; ///< The normal in the hit point interpolated by the vertex normals of the triangles
	CRTColor barycentricCoordinates; ///< Where the triangle is hit
	CRTMaterial material; ///< The material of the hit object
	bool isValid; ///< If the structure instance is valid

	void set(const size_t& idxGeometryObject,
		const size_t& idxTriangleI,
		const CRTTriangle& triangle,
		const CRTTriangle::retDataFromTriangleIntersect intersectData,
		const CRTMaterial& geometryObjectMaterial,
		const std::vector<CRTVector>& vertexNormals,
		const std::vector<size_t>& faces
	) {
		idxGeometry = idxGeometryObject;
		idxTriangle = idxTriangleI;
		p = intersectData.p;
		t = intersectData.t;
		triangleFace = triangle.getFaceNormal();
		const size_t idx = idxTriangleI * 3;
		interpolatedVerticesNormal =
			(vertexNormals[faces[idx + 1]] * intersectData.barycentricCoordinates.x +
				vertexNormals[faces[idx + 2]] * intersectData.barycentricCoordinates.y +
				vertexNormals[faces[idx]] * intersectData.barycentricCoordinates.z).normalize();
		barycentricCoordinates = intersectData.barycentricCoordinates;
		material = geometryObjectMaterial;
		isValid = true;
	}
};

#endif // !INTERSECTIONDATA_H
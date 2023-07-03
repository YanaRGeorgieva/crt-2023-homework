#include "CRTMesh.h"

void CRTMesh::generateGeometry() {
	triangles.reserve(faces.size() / AXIS_COUNT + 1);
	vertexNormals.reserve(vertices.size());
	vertexNormals.resize(vertices.size());
	// In interval [0,1)
	size_t lenFaces = faces.size();
	for (size_t i = 0; i < lenFaces; i += AXIS_COUNT) {
		triangles.emplace_back(CRTTriangle(vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]], CRTUintVector{ faces[i], faces[i + 1], faces[i + 2] }));
		vertexNormals[faces[i]] += triangles.back().getFaceNormal();
		vertexNormals[faces[i + 1]] += triangles.back().getFaceNormal();
		vertexNormals[faces[i + 2]] += triangles.back().getFaceNormal();
	}

	size_t lenVertices = vertices.size();
	for (size_t i = 0; i < vertices.size(); i++) {
		vertexNormals[i] = vertexNormals[i].normalize();
	}

	calculateAABB();
	constructBVHTree();
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

const size_t& CRTMesh::getMaterialIdx() const {
	return materialIndex;
}

const CRTBox& CRTMesh::getBox() const {
	return box;
}

void CRTMesh::setMaterial(const CRTMaterial& material) {
	this->material = material;
}

void CRTMesh::calculateAABB() {
	const std::vector<CRTVector>& vertices = getVertices();
	for (auto& vertex : vertices) {
		// Expand box with vertex.
		box.expand(vertex);
	}
}

CRTIntersectionData CRTMesh::intersect(const CRTRay& ray,
	const size_t idxGeometryObject,
	float& bestT) const {
	CRTIntersectionData intersectionPointInfo{};
	intersectionPointInfo.isValid = false;
	const size_t len = triangles.size();
	if (box.isValid() && !box.intersect(ray, bestT).isValid) {
		return intersectionPointInfo;
	}
	for (size_t i = 0; i < len; i++) {
		const CRTTriangle& tri = triangles[i];
		const CRTTriangle::retDataFromTriangleIntersect& intersectData = tri.intersect(ray, bestT);
		//	If P is on the left of the 3 edges and t > 0, we have an intersection
		if (intersectData.isValid) {
			if (ray.type == RayType::shadow) {
				// One intersection suffices
				intersectionPointInfo.set(idxGeometryObject, i, tri, intersectData, material, vertexNormals, faces);
				return intersectionPointInfo;
			}
			// Get closest p (with least length)
			bestT = intersectData.t;
			intersectionPointInfo.set(idxGeometryObject, i, tri, intersectData, material, vertexNormals, faces);
		}
	}
	return intersectionPointInfo;
}

void CRTMesh::constructBVHTree() {
	bvhTree.clear();
	bvhTree.reserve(triangles.size() / MAX_TRIANGLES_PER_LEAF * 2 + 1);
	CRTAccTreeNode& root = bvhTree.emplace_back();
	root.aabb = box; // Must be calculated before that
	root.parent = -1;
	root.startTriangleIdx = 0;
	if (triangles.size() > 0) {
		root.endTriangleIdx = int(triangles.size() - 1);
	} else {
		root.endTriangleIdx = 0;
	}
	root.children[0] = -1;
	root.children[1] = -1;
	constructBVHTreeRecursively(0, 0);
}

void CRTMesh::constructBVHTreeRecursively(int nodeIdx, int depth) {
	if (nodeIdx > 0) {
		// Calculate the bounding box.
		for (int i = bvhTree[nodeIdx].startTriangleIdx; i < bvhTree[nodeIdx].endTriangleIdx + 1; i++) {
			const CRTUintVector& verticesIndexes = triangles[i].getVerticesFacesIndices();

			// Expand the bounding box based on the triangle vertices. It will fit perfectly all the triangles.
			for (int axis = 0; axis < AXIS_COUNT; axis++) {
				bvhTree[nodeIdx].aabb.expand(vertices[verticesIndexes[axis]]);
			}
		}
	}

	const size_t trianglesLength = bvhTree[nodeIdx].endTriangleIdx - bvhTree[nodeIdx].startTriangleIdx + 1;

	// If the leaf criteria is met or max depth of tree is met, stop recursion and return.
	if (trianglesLength <= MAX_TRIANGLES_PER_LEAF || depth >= MAX_TREE_DEPTH) {
		return;
	}

	// Calculate the axis to split along.
	int splitAxis = depth % AXIS_COUNT;

	// Calculate the midpoint to split the triangles. We will try to create a full balanced tree. 
	int mid = trianglesLength / 2;

	// Sort the triangles based on their centroid along the chosen axis.
	std::sort(triangles.begin() + bvhTree[nodeIdx].startTriangleIdx, triangles.begin() + bvhTree[nodeIdx].endTriangleIdx + 1,
		[&](const CRTTriangle& a, const CRTTriangle& b) {
			return lessThan(a.getCentroid()[splitAxis], b.getCentroid()[splitAxis]);
		});

	// Create the left child node.
	CRTAccTreeNode& leftChild = bvhTree.emplace_back();
	bvhTree[nodeIdx].children[0] = int(bvhTree.size() - 1);

	leftChild.parent = nodeIdx;
	leftChild.children[0] = -1;
	leftChild.children[1] = -1;
	leftChild.startTriangleIdx = bvhTree[nodeIdx].startTriangleIdx;
	leftChild.endTriangleIdx = bvhTree[nodeIdx].startTriangleIdx + mid;

	// Create the right child node.
	CRTAccTreeNode& rightChild = bvhTree.emplace_back();
	bvhTree[nodeIdx].children[1] = int(bvhTree.size() - 1);

	rightChild.parent = nodeIdx;
	rightChild.children[0] = -1;
	rightChild.children[1] = -1;
	rightChild.startTriangleIdx = bvhTree[nodeIdx].startTriangleIdx + mid + 1;
	rightChild.endTriangleIdx = bvhTree[nodeIdx].endTriangleIdx;
	
	// Recursive calls for children.
	constructBVHTreeRecursively(bvhTree[nodeIdx].children[0], depth + 1);
	constructBVHTreeRecursively(bvhTree[nodeIdx].children[1], depth + 1);
}

CRTIntersectionData CRTMesh::intersectBVHTree(const CRTRay& ray,
	const size_t idxGeometryObject,
	float& bestT) const {
	CRTIntersectionData intersectionPointInfo{};
	intersectionPointInfo.isValid = false;

	intersectionPointInfo = intersectBVHTreeRecursively(ray, idxGeometryObject, bestT, bvhTree[0]);
	if (intersectionPointInfo.isValid) {
		if (lessThan(intersectionPointInfo.t, bestT)) {
			bestT = intersectionPointInfo.t;
		}
	}
	return intersectionPointInfo;
}

CRTIntersectionData CRTMesh::intersectBVHTreeRecursively(const CRTRay& ray, const size_t idxGeometryObject, const float& bestT, const CRTAccTreeNode& node) const {
	CRTIntersectionData intersectionPointInfo{};
	intersectionPointInfo.isValid = false;

	if (node.aabb.isValid() && !node.aabb.intersect(ray, bestT).isValid) {
		return intersectionPointInfo;
	}

	if (node.isLeaf()) {
		float currentbestT = bestT;
		for (int i = node.startTriangleIdx; i < node.endTriangleIdx + 1; i++) {
			const CRTTriangle& tri = triangles[i];
			const CRTTriangle::retDataFromTriangleIntersect& intersectData = tri.intersect(ray, currentbestT);
			//	If P is on the left of the 3 edges and t > 0, we have an intersection
			if (intersectData.isValid) {
				if (ray.type == RayType::shadow) {
					// One intersection suffices
					intersectionPointInfo.set(idxGeometryObject, -1, tri, intersectData, material, vertexNormals, faces);
					return intersectionPointInfo;
				}
				// Get closest p (with least length)
				currentbestT = intersectData.t;
				intersectionPointInfo.set(idxGeometryObject, -1, tri, intersectData, material, vertexNormals, faces);
			}
		}
	} else {
		CRTIntersectionData leftIntersectionPointInfo =
			intersectBVHTreeRecursively(ray, idxGeometryObject, bestT, bvhTree[node.children[0]]);
		CRTIntersectionData rightIntersectionPointInfo =
			intersectBVHTreeRecursively(ray, idxGeometryObject, bestT, bvhTree[node.children[1]]);

		if (leftIntersectionPointInfo.isValid && rightIntersectionPointInfo.isValid) {
			if (lessThan(leftIntersectionPointInfo.t, rightIntersectionPointInfo.t)) {
				intersectionPointInfo = leftIntersectionPointInfo;
			} else {
				intersectionPointInfo = rightIntersectionPointInfo;
			}
		} else if (leftIntersectionPointInfo.isValid) {
			intersectionPointInfo = leftIntersectionPointInfo;
		} else if (rightIntersectionPointInfo.isValid) {
			intersectionPointInfo = rightIntersectionPointInfo;
		}

	}
	return intersectionPointInfo;
}
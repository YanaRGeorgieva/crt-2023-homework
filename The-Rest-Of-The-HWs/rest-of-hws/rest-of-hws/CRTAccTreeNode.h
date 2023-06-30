#ifndef CRTACCTREENODE_H
#define CRTACCTREENODE_H

#include <vector>

#include "CRTBox.h"
#include "CRTMaterial.h"
#include "CRTTriangle.h"
#include "CRTIntersectionData.h"

/// Node for the acceleration tree structure,
/// holding a box for the sub space the node represents and indices for the children sub spaces represented by nodes
struct CRTAccTreeNode {
	CRTAccTreeNode() : aabb() {
		triangles.clear();
		parent = -1;
		children[0] = -1;
		children[1] = -1;
	};

	///// Intersect the ray with the triangles in the box, meaningful only for nodes, the others do not have triangles
	//void intersect(
	//	const CRTRay& ray,
	//	const float maxT,
	//	const std::vector<CRTMaterial>& materials,
	//	float& t,
	//	float& minT,
	//	CRTIntersectionData& data
	//) const;

	bool isLeaf() const {
		return children[0] == -1 && children[1] == -1;
	}

	std::vector<CRTTriangle> triangles; ///< In case of a leaf node a list with the traingles for intersection in the box for the node, empty otherwise
	CRTBox aabb; ///< Axis aligned box for the sub space this node represents
	int children[2]; ///< The left and right indices for the node's children, indexing in the big list for the tree with all the nodes
	int parent; ///< The index of the parent node for this node and ints sub space
};

#endif // !CRTACCTREENODE_H

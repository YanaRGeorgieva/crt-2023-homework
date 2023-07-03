#ifndef CRTACCTREENODE_H
#define CRTACCTREENODE_H

#include <vector>

#include "CRTBox.h"
#include "CRTRay.h"
#include "CRTMaterial.h"
#include "CRTTriangle.h"
#include "CRTIntersectionData.h"

/// Node for the acceleration tree structure,
/// holding a box for the sub space the node represents and indices for the children sub spaces represented by nodes.
struct CRTAccTreeNode {
	CRTAccTreeNode() : aabb() {
		parent = -1;
		startTriangleIdx = -1;
		endTriangleIdx = -1;
		children[0] = -1;
		children[1] = -1;
	};

	bool isLeaf() const;

	int startTriangleIdx, endTriangleIdx; ///< Start index and end Index of triangles for current box.
	CRTBox aabb; ///< Axis aligned box for the sub space this node represents.
	int children[2]; ///< The left and right indices for the node's children, indexing in the big list for the tree with all the nodes.
	int parent; ///< The index of the parent node for this node and ints sub space.
};

#endif // !CRTACCTREENODE_H

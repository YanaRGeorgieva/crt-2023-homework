#include "CRTAccTreeNode.h"

bool CRTAccTreeNode::isLeaf() const {
	return children[0] == -1 && children[1] == -1;
}
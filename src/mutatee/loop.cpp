/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   loop.cpp
 * @author Jan Mussler
 * @brief  Loop node implementation
 */

#include "mutatee.h"

using namespace gi::mutatee;

TreeNode::TreeNode() : node(0) {

}

BPatch_loopTreeNode* TreeNode::getBPNode() {
    return node;
}

std::vector<LoopTreeNode> TreeNode::getChildren() {
    std::vector<LoopTreeNode> children;
    assert(node);
    for (std::vector<BPatch_loopTreeNode*>::iterator i = node->children.begin();
            i != node->children.end();
            i++
            ) {
        children.push_back(LoopTreeNode((*i)));
    }
    return children;
}

LoopTree::LoopTree(Function f) : TreeNode() {
    node = f.getBPFunction()->getCFG()->getLoopTree();
}

std::string LoopTreeNode::getName() {
    return std::string(node->name());
}

LoopTreeNode::LoopTreeNode(BPatch_loopTreeNode* bpNode) : TreeNode() {
    node = bpNode;
}

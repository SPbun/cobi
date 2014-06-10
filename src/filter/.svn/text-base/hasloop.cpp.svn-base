/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   hasloop.cpp
 * @author Jan Mussler
 * @brief Implementation of the hasloop property
 */

#include "implproperties.h"
#include <stack>

using namespace std;
using namespace gi::mutatee;
using namespace gi::filter;
using namespace gi::filter::properties;
namespace gix = gi::xml;

HasLoop::HasLoop() {

}

HasLoop::~HasLoop() {
    
}

bool HasLoop::init(Mutatee* mutatee) {
    return true;
}

void countLevel(unsigned int level,unsigned int currentLevel, unsigned int &count, BPatch_loopTreeNode* node) {
    if(level == currentLevel) {
        count += node->children.size();
        return;
    }
    
    if(level > currentLevel) {
        for(unsigned int i = 0; i < node->children.size(); ++i) {
            countLevel(level,currentLevel+1,count,node->children[i]);
        }
    }
}

bool HasLoop::match(gi::mutatee::IFunction* f) {
    BPatch_function* func = f->getBPFunction();
    if(func==0) return false;

    unsigned int count = 0;

    BPatch_loopTreeNode* root = func->getCFG()->getLoopTree();

    countLevel(level,1,count,root);

    return count >= minValue && ( maxValue==0 || count < maxValue );
}

bool HasLoop::setUp(gix::FilterParser* fp, DOMElement* headElement) {

    minValue = gix::getUIntAttributeValue(headElement, "minValue");
    maxValue = gix::getUIntAttributeValue(headElement, "maxValue");
    level = gix::getUIntAttributeValue(headElement, "level");
    
    return true;
}

bool HasLoop::needsInit() {
    return false;
}

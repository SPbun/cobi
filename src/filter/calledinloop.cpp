/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   calledinloop.cpp
 * @author Jan Mussler
 * @brief Implementation of called in loop property
 */

#include "implproperties.h"
#include "callgraph.h"
#include "mutatee.h"
#include <stack>

using namespace std;
using namespace gi::filter;
using namespace gi::filter::properties;
using namespace gi::mutatee;
using namespace gi::mutatee::graph;
namespace gix = gi::xml;

CalledInLoop::CalledInLoop() : mutatee(0) {

}

CalledInLoop::~CalledInLoop() {

}

bool CalledInLoop::init(Mutatee* m) {
    mutatee = m;
    return true;
}

string getMangledNameThroughBuffer(BPatch_function* func) {
    char buffer[255];
    buffer[254] = 0;
    func->getMangledName(buffer,254);
    return string(buffer);
}

/* traverse loop tree, return true if function is called from this loop */
bool CalledInLoop::checkLevel(const string &mangledName, unsigned int currentLevel, BPatch_loopTreeNode* node) {
    bool result = false;

    if( currentLevel >= level ) {
        if(node->numCallees() > 0) {

            vector<BPatch_function*> funcs;
            
            node->getCallees(funcs,mutatee->getAddressSpace());

            for(unsigned int i = 0; i < funcs.size(); ++i) {
                if( getMangledNameThroughBuffer(funcs[i]) == mangledName ) {
                    return true;
                }
            }
        }
    }

    for(unsigned int i = 0; i < node->children.size(); ++i) {
        result =  result || checkLevel(mangledName,currentLevel+1,node->children[i]);
    }

    return result;
}

bool CalledInLoop::match(gi::mutatee::IFunction* f) {
    assert(mutatee!=0);
    Node* node = mutatee->getCallGraph().getNodeForName(f->getMangledName());
    if(node==0) {
        // calls through pointers return 0 node
        return false;
    }

    string target = f->getMangledName();

    bool result = false;

    set<Node*> inc = node->getIncomingNodes();

    for(set<Node*>::iterator i = inc.begin(); i != inc.end(); ++i) {
        BPatch_loopTreeNode* root = (*i)->getFunction()->getBPFunction()->getCFG()->getLoopTree();
        // 0 start level, root node contains all loops in the function
        result = result || checkLevel(target,0,root);
        if(result) {
            break;
        }
    }    

    return result;
}

bool CalledInLoop::setUp(gix::FilterParser* fp, DOMElement* headElement) {
    level = gix::getUIntAttributeValue(headElement, "level");

    return true;
}

bool CalledInLoop::needsInit() {
    return true;
}

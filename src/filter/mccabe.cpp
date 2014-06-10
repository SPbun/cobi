/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   mccabe.cpp
 * @author Jan Mussler
 * @brief Implementation of Cyclomatic Complexity property
 */

#include "implproperties.h"

using namespace std;
using namespace gi::mutatee;
using namespace gi::filter::properties;
namespace gix = gi::xml;

Mccabe::Mccabe() {
    minValue = 0;
    maxValue = 0;
}

bool Mccabe::setUp(gix::FilterParser* fp, DOMElement* headElement) {
     minValue = gix::getNumericValue(headElement, "minValue");
     maxValue = gix::getNumericValue(headElement, "maxValue");
     return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool Mccabe::init(Mutatee* mutatee) {
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool Mccabe::needsInit() {
    return false;
}

typedef set<BPatch_basicBlock*> setBPBlock;

bool Mccabe::match(gi::mutatee::IFunction* f) {
    BPatch_function* func = f->getBPFunction();
    if (func == 0) {
        return false;
    }

    BPatch_flowGraph* fg = func->getCFG();

    setBPBlock blocks;
    fg->getAllBasicBlocks(blocks);

    // VAL = EDGES - BLOCKS + 1

    size_t val = 1;
    vector<BPatch_edge*> outEdges;
    for (setBPBlock::iterator i = blocks.begin();
            i != blocks.end();
            i++) {

        (*i)->getOutgoingEdges(outEdges);        
        val += outEdges.size();
        outEdges.clear();
    }

    val -= blocks.size();

    return val >= minValue && ( maxValue == 0 || val < maxValue);
}

Mccabe::~Mccabe() {
}

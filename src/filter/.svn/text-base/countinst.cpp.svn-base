/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   countinst.cpp
 * @author Jan Mussler
 * @brief Implementation of number of instruction property
 */

#include "implproperties.h"

using namespace std;
using namespace gi::filter::properties;
using namespace gi::mutatee;
namespace gix = gi::xml;

CountInst::CountInst() {
    minValue = 0;
    maxValue = 0;
}

bool CountInst::setUp(gix::FilterParser* fp, DOMElement* headElement) {
     minValue = gix::getNumericValue(headElement, "minValue");
     maxValue = gix::getNumericValue(headElement, "maxValue");
     return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool CountInst::init(Mutatee* mutatee) {
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool CountInst::needsInit() {
    return false;
}

typedef set<BPatch_basicBlock*> setBPBlock;

bool CountInst::match(gi::mutatee::IFunction* f) {
    BPatch_function* func = f->getBPFunction();
    if (func == 0) {
        return false;
    }

    unsigned int val = 0;

    BPatch_flowGraph* fg = func->getCFG();

    setBPBlock blocks;
    fg->getAllBasicBlocks(blocks);

    for (setBPBlock::iterator i = blocks.begin();
            i != blocks.end();
            i++) {
            vector<Dyninst::InstructionAPI::Instruction::Ptr> instructions;
            (*i)->getInstructions(instructions);

            val += instructions.size();
    }

    return val >= minValue && ( maxValue == 0 || val < maxValue);
}

CountInst::~CountInst() {
}

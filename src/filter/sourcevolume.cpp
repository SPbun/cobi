/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   sourcevolume.cpp
 * @author Jan Mussler
 * @brief Implementation of the Lines of Code metric property
 */

#include "implproperties.h"

using namespace std;
using namespace gi::filter::properties;
using namespace gi::mutatee;
namespace gix = gi::xml;

SourceVolume::SourceVolume() {
}

/**
 * used to setup a property with the domelement from the filter spec
 */
bool SourceVolume::setUp(gix::FilterParser* fp, DOMElement* headElement) {
     minValue = gix::getNumericValue(headElement, "minValue");
     maxValue = gix::getNumericValue(headElement, "maxValue");
     return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool SourceVolume::init(Mutatee* mutatee) {
    this->mutatee = mutatee;
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool SourceVolume::needsInit() {
    return true;
}

bool SourceVolume::match(gi::mutatee::IFunction* f) {
    unsigned int val;

    BPatch_function* func = f->getBPFunction();
    if( func == 0 ) {
        return 0;
    }

    vector<BPatch_basicBlock*> entry;
    vector<BPatch_basicBlock*> exit;
    
    func->getCFG()->getEntryBasicBlock(entry);
    func->getCFG()->getExitBasicBlock(exit);

    // used to exclude, so if not working, do not exclude
    if(entry.size()==0 || exit.size()==0) return false;

    SourceLine sl = mutatee->getFirstSourceLineOfAddress(entry[0]->getStartAddress());

    unsigned long endAddress = 0;
    unsigned long tmp = 0;

    // get last address of function, want all source lines, not first return statement
    for(vector<BPatch_basicBlock*>::iterator i = exit.begin(); i != exit.end(); i++) {
        tmp = (*i)->getLastInsnAddress();
        if(tmp > endAddress) {
            endAddress = tmp;
        }
    }

    SourceLine el = mutatee->getFirstSourceLineOfAddress(endAddress);

    // used to exclude, so if not working, do not exclude
    if(el.line==0 || sl.line==0) {
        return false;
    }

    val = static_cast<int>(el.line - sl.line);

    return val > minValue && ( maxValue == 0 || val < maxValue);
}

SourceVolume::~SourceVolume() {
}

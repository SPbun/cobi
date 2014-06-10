/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   exitpoints.cpp
 * @author Jan Mussler
 * @brief Implementation of no exit point property
 */

#include "implproperties.h"


using namespace std;
using namespace gi::mutatee;
using namespace gi::filter::properties;
namespace gix = gi::xml;

CountExits::CountExits() {
    minValue = 0;
    maxValue = 0;
}

bool CountExits::setUp(gix::FilterParser* fp, DOMElement* headElement) {
     minValue = gix::getNumericValue(headElement, "minValue");
     maxValue = gix::getNumericValue(headElement, "maxValue");
     return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool CountExits::init(Mutatee* mutatee) {
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool CountExits::needsInit() {
    return false;
}

bool CountExits::match(gi::mutatee::IFunction* f) {    
        // vector<BPatch_point*>* enterPoints = function->getFunction()->getBPFunction()->findPoint(BPatch_entry);

    vector<BPatch_point*>* exitPoints = f->getBPFunction()->findPoint(BPatch_exit);

    unsigned int val = exitPoints->size();    
    delete exitPoints;

    return val >= minValue && ( maxValue == 0 || val < maxValue);
}

CountExits::~CountExits() {
}

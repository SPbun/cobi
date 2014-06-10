/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   enterpoints.cpp
 * @author Jan Mussler
 * @brief Implementation of no enter point property
 */

#include "implproperties.h"

using namespace std;
using namespace gi::mutatee;
using namespace gi::filter::properties;
namespace gix = gi::xml;

CountEnter::CountEnter() {
    minValue = 0;
    maxValue = 0;
}

bool CountEnter::setUp(gix::FilterParser* fp, DOMElement* headElement) {
     minValue = gix::getNumericValue(headElement, "minValue");
     maxValue = gix::getNumericValue(headElement, "maxValue");
     return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool CountEnter::init(Mutatee* mutatee) {
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool CountEnter::needsInit() {
    return false;
}

bool CountEnter::match(gi::mutatee::IFunction* f) {
    vector<BPatch_point*>* enterPoints = f->getBPFunction()->findPoint(BPatch_entry);

    unsigned int val = enterPoints->size();
    delete enterPoints;

    return val >= minValue && ( maxValue == 0 || val < maxValue);
}

CountEnter::~CountEnter() {
}

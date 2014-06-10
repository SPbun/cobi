/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   countcallsites.cpp
 * @author Jan Mussler
 * @brief Implementation of number of callsites properties
 */

#include "implproperties.h"
#include "filterparser.h"

using namespace std;
using namespace gi::filter;
using namespace gi::filter::properties;

namespace gix = gi::xml;

CountCallSites::CountCallSites() {
}

CountCallSites::~CountCallSites() {
    delete rule;
}

bool CountCallSites::needsInit() {
    return false;
}

bool CountCallSites::match(gi::mutatee::IFunction* f) {
    vector<IFunction*> calls = f->getSubCalls();

    unsigned int count = 0;

    for (vector<IFunction*>::iterator i = calls.begin(); i != calls.end(); i++) {
        if (rule->match(*i)) {
            count++;
        }
    }
    return count >= minValue && ( maxValue == 0 || count < maxValue);
}

bool CountCallSites::setUp(gix::FilterParser* fp, DOMElement* headElement) {

    minValue = gix::getUIntAttributeValue(headElement, "minValue");
    maxValue = gix::getUIntAttributeValue(headElement, "maxValue");

    gi::filter::AND* andRule = new AND();

    fp->parseNodeContainingRules(andRule, headElement);

    if (!andRule->count() > 0) {
        delete andRule;
        rule = new RTrue();
    }
    else {
        rule = andRule;
    }

    return true;
}

bool CountCallSites::init(Mutatee* mutatee) {
    return true;
}

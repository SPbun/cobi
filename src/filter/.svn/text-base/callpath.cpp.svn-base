/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   callpath.cpp
 * @author Jan Mussler
 * @brief Implementation of callpath property
 */

#include "implproperties.h"
#include "filterparser.h"

using namespace std;
using namespace gi::filter::properties;
namespace gix = gi::xml;

/**
 * used to setup a property with the domelement from the filter spec
 */
bool CallPath::setUp(gix::FilterParser* fp, DOMElement* headElement) {
    AND* set = new AND();
    fp->parseNodeContainingRules(set, headElement);
    targetRule = set;
    string print = gi::xml::getAttributeValue(headElement,"show");
    string direction = gi::xml::getAttributeValue(headElement,"direction");

    if ( print == "true" ) {
        printSetAfterInit = true;
    }
    else {
        printSetAfterInit = false;
    }
    
    if ( direction == "forward" ) {
        forward = true;
    }
    else {
        forward = false;
    }


    return true;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool CallPath::init(Mutatee* mutatee) {
    gi::mutatee::graph::CallGraph& gr = mutatee->getCallGraph();
    if(forward==false) {
        namesOnPath = gr.getFunctionNamesOnPathToRule(targetRule);
    }
    else {
        namesOnPath = gr.getFunctionNamesOnPathFromRule(targetRule);
    }

    if(printSetAfterInit) {
        graph::displayQResult(namesOnPath);
    }
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool CallPath::needsInit() {
    return true;
}

bool CallPath::match(gi::mutatee::IFunction* f) {
    return namesOnPath->find(f->getMangledName()) != namesOnPath->end();
}

CallPath::~CallPath() {
    delete targetRule;
    delete namesOnPath;
}

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   depth.cpp
 * @author Jan Mussler
 * @brief Implementation of Depth property
 */

#include "implproperties.h"
#include "filterparser.h"

using namespace std;
using namespace gi::filter::properties;
namespace gix = gi::xml;

/**
 * used to setup a property with the domelement from the filter spec
 */
bool Depth::setUp(gix::FilterParser* fp, DOMElement* headElement) {
    originName = gix::getAttributeValue(headElement,"origin");
    stringstream sDepth(gix::getAttributeValue(headElement,"depth"));
    sDepth >> depth;

    if( originName != "" && depth > 0) {
        return true;
    }
    return false;
}

/**
 * used to do one time calculation before filtering the mutatee
 */
bool Depth::init(Mutatee* mutatee) {
    std::cout << "creating depth filter(" << originName << ", " << depth << ")\n";
    namesWithinDepth = mutatee->getCallGraph().getFunctionsWithinDepthOf(originName,depth);
    return true;
}

/**
 * if property needs initialisation, return true
 */
bool Depth::needsInit() {
    return true;
}

bool Depth::match(gi::mutatee::IFunction* f) {
    return namesWithinDepth->find(f->getName()) != namesWithinDepth->end();
}

Depth::~Depth() {
    delete namesWithinDepth;
}

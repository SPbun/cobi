/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   countcallee.cpp
 * @author Jan Mussler
 * @brief Implementation of count callees property
 */

#include "implproperties.h"
#include "filterparser.h"
#include "callgraph.h"

using namespace std;
using namespace gi::filter;
using namespace gi::filter::properties;
using namespace gi::mutatee::graph;

namespace gix = gi::xml;

CountCallees::CountCallees() {
}

CountCallees::~CountCallees() {
    delete rule;
}

bool CountCallees::needsInit() {
    return true;
}

bool CountCallees::match(gi::mutatee::IFunction* f) {

    unsigned int count = 0;
    Node* node = mutatee->getCallGraph().getNodeForName(f->getMangledName());
    if(node==0) {
        std::cout << "waring: callgraph node not found for [" << f->getMangledName() <<"]\n";
        return false;
    }

    if (rule == 0) {
        count = node->countIncoming();
    }
    else {
        set<Node*> inc = node->getIncomingNodes();

        for (set<Node*>::iterator i = inc.begin(); i != inc.end(); i++) {
            IFunction* func = (*i)->getFunction();
            assert((*i)->getFunction() != 0);
            if (func != 0 && rule->match(func)) {
                count++;
            }
        }
    }

    return (count >= minValue) && (maxValue == 0 || count < maxValue);
}

bool CountCallees::setUp(gix::FilterParser* fp, DOMElement* headElement) {

    minValue = gix::getUIntAttributeValue(headElement, "minValue");
    maxValue = gix::getUIntAttributeValue(headElement, "maxValue");

    gi::filter::AND* andRule = new AND();
    fp->parseNodeContainingRules(andRule, headElement);

    if (!(andRule->count() > 0)) {
        delete andRule;
        rule = 0;
    }
    else {
        rule = andRule;
    }

    return true;
}

bool CountCallees::init(Mutatee* mutatee) {
    this->mutatee = mutatee;
    return true;
}

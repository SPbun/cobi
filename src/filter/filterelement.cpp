/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filterelement.cpp
 * @author Jan Mussler
 * @brief Implementation of the filter element
 *
 * A filter element is either include/exclude
 */

#include "filters.h"

using namespace gi::filter;

FilterElement::FilterElement() {

}

FilterElement::FilterElement(IRule* aRuleTree) {
    assert(aRuleTree);
    filterRuleTree = aRuleTree;
}

FilterElement::~FilterElement() {
    delete filterRuleTree;
    filterRuleTree = 0;
}

IRule* FilterElement::getRule() {
    return filterRuleTree;
}

filterResult* FilterElement::getResult(filterResult* baseSet, filterResult* currentSet) {
    filterResult* result = new filterResult();
    
    for (filterResult::iterator f = currentSet->begin(); f != currentSet->end(); f++) {
        if (filterRuleTree->match((*f)->getFunction())) {
            result->insert((*f));
  //          std::cout << "\t\tadding function: " << (*f)->getFunction()->getFunctionName() << " " << (*f)->getFunction()->getNameSpace() << " " << (*f)->getFunction()->getClassName() << " | "<< (*f)->getFunction()->getName() << " "  << "\n";
        }
        else {
  //          std::cout << "\t\tskipping function: " << (*f)->getFunction()->getFunctionName() << " " << (*f)->getFunction()->getNameSpace() << " " << (*f)->getFunction()->getClassName() << " | "<< (*f)->getFunction()->getName() << " "  << "\n";
        }
    }
    return result;
}

IncludeElement::IncludeElement(IRule* aRuleTree) : FilterElement(aRuleTree) {
}

filterResult* IncludeElement::getResult(filterResult* baseSet, filterResult* currentSet) {
    filterResult* result = new filterResult();

    // copy old set
    result->insert(currentSet->begin(), currentSet->end());

    // add new functions from base set
    for (filterResult::iterator f = baseSet->begin(); f != baseSet->end(); f++) {
        if (filterRuleTree->match((*f)->getFunction())) {
            result->insert((*f));
//            std::cout << "\t\tadding function: " << (*f)->getFunction()->getFunctionName() << " " << (*f)->getFunction()->getNameSpace() << " " << (*f)->getFunction()->getClassName() << " | "<< (*f)->getFunction()->getName() << " "  << "\n";
        }
        else {
//            std::cout << "\t\tskipping function: " << (*f)->getFunction()->getFunctionName() << " " << (*f)->getFunction()->getNameSpace() << " " << (*f)->getFunction()->getClassName() << " | "<< (*f)->getFunction()->getName() << " "  << "\n";
        }
    }

    return result;
}

ExcludeElement::ExcludeElement(IRule* aRuleTree) : FilterElement(aRuleTree) {
}

filterResult* ExcludeElement::getResult(filterResult* baseSet, filterResult* currentSet) {
    filterResult* result = new filterResult();

    for (filterResult::iterator f = currentSet->begin(); f != currentSet->end(); f++) {
        assert((*f)->getFunction() != 0);
        if (filterRuleTree->match((*f)->getFunction())) {
          //  std::cout << "\t\tremoving function: " << (*f)->getFunction()->getName() << " " << (*f)->getFunction()->getMangledName() << " " << (*f)->getFunction()->getModuleName() <<  "\n";
        }
        else {
            result->insert((*f));
          //  std::cout << "\t\tkeeping function: " << (*f)->getFunction()->getName() << " " << (*f)->getFunction()->getMangledName()  << " " << (*f)->getFunction()->getModuleName() << "\n";
        }
    }
    return result;
}

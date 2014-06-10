/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   patterns.cpp
 * @author Jan Mussler
 * @brief Implementation of pattern related base class
 */

#include "patterns.h"

using namespace gi::mutatee;
using namespace gi::filter;

IMatchString::IMatchString() {
}

IPattern::IPattern(IMatchString* strategy) : matchStrategy(strategy) {

}

IPatterns::IPatterns(IMatchString* strategy) : IPattern(strategy) {
    
}

void IPatterns::addPattern(std::string aPattern) {
    patterns.push_back(aPattern);
}

bool IPatterns::match(gi::mutatee::IFunction* f) {
    for (std::vector<std::string>::const_iterator i = patterns.begin();
            i != patterns.end();
            i++) {
        if (matchStrategy->match((*i), getName(f))) {
            return true;
        }
    }
    return false;
}

/**
 * FunctionNames class
 */
FunctionNames::FunctionNames(IMatchString* strategy) : IPatterns(strategy) {

}

std::string FunctionNames::getName(gi::mutatee::IFunction* f) {
    return f->getFunctionName();
}

/**
 * Names class
 */
Names::Names(IMatchString* strategy) : IPatterns(strategy) {

}

std::string Names::getName(gi::mutatee::IFunction* f) {
    return f->getName();
}

/**
 * ClassNames class
 */
ClassNames::ClassNames(IMatchString* strategy) : IPatterns(strategy) {

}

std::string ClassNames::getName(gi::mutatee::IFunction* f) {
    return f->getClassName();
}

/**
 * ClassNames class
 */
Namespaces::Namespaces(IMatchString* strategy) : IPatterns(strategy) {

}

std::string Namespaces::getName(gi::mutatee::IFunction* f) {
    return f->getNameSpace();
}

/**
 * ModuleNames Class
 */
ModuleNames::ModuleNames(IMatchString* strategy) : IPatterns(strategy) {

}

std::string ModuleNames::getName(gi::mutatee::IFunction* f) {
    return f->getModuleName();
}

/**
 * Constructor
 * @param aName function name
 */
FunctionName::FunctionName(IMatchString* strategy, std::string aName) : IPattern(strategy), name(aName) {

}

/**
 * check for match
 * @param f function to be checked
 */
bool FunctionName::match(IFunction* f) {
    return matchStrategy->match(name, f->getName());
}

/**
 * Constructor
 * @param aName module name
 */
ModuleName::ModuleName(IMatchString* strategy, std::string aName) : IPattern(strategy), name(aName) {
}

/**
 * check for match of module name
 * @param f function to be checked
 */
bool ModuleName::match(IFunction* f) {
    return matchStrategy->match(name, f->getModuleName());
}

std::map<MatchStrategyFactory::strategies, IMatchString*> MatchStrategyFactory::strategymap;

/**
 * get pointer to the selected strategy object
 */
IMatchString* const MatchStrategyFactory::getStrategy(strategies s) {

    if (strategymap.size() == 0) {
        strategymap[EQUALS] = new MatchEquals();
        strategymap[SIMPLE] = new MatchSimplePattern();
        strategymap[REGEX] = new MatchRegex();
        strategymap[PREFIX] = new MatchPrefix();
        strategymap[SUFFIX] = new MatchSuffix();
        strategymap[FIND] = new MatchFind();
    }

    return strategymap[s];
}

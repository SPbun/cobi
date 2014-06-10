/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   contextconstant.cpp
 * @author Jan Mussler
 * @brief Implements a @@ Context Variable AST node
 */

#include "generator.h"

using namespace gi::generator;

/**
 * @param aName Variable name
 */
ContextConstant::ContextConstant(std::string aName) : name(aName) {

}

std::string ContextConstant::getStringValue(IContext* c) {
    return c->getStrValue(name);
}

/**
 * return variable snippet from context
 */
BPatch_snippet* ContextConstant::getSnippet(IContext* c) {
    return c->getVariable(name);
}

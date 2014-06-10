/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   gvariable.cpp
 * @author Jan Mussler
 * @brief Implements an Variable AST node
 */

#include "generator.h"

using namespace gi::generator;

/**
 * @param aName Variable name
 */
Variable::Variable(std::string aName) : name(aName) {
}

/**
 * return variable snippet from context
 */
BPatch_snippet* Variable::getSnippet(IContext* c) {
    return c->getVariable(name);
}

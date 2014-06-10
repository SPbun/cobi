/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   functioncall.cpp
 * @author Jan Mussler
 * @brief Implements a Functioncall AST node
 */

#include "generator.h"

using namespace gi::generator;

FunctionCall::FunctionCall(std::string name) : functionName(name) {
    
}

FunctionCall::~FunctionCall() {
    for (std::vector<IExpression*>::iterator i = parameters.begin();
            i != parameters.end();
            i++) {
        delete (*i);
    }
    parameters.clear();
}

void FunctionCall::addParameter(IExpression* e) {
    parameters.push_back(e);
}

BPatch_snippet* FunctionCall::getSnippet(IContext* c) {
    std::vector<BPatch_snippet*> args;

    for (std::vector<IExpression*>::const_iterator i = parameters.begin();
            i != parameters.end();
            i++) {
        args.push_back((*i)->getSnippet(c));
    }

    BPatch_function* function = c->getFunction(functionName);
    return new BPatch_funcCallExpr(*function, args);
}

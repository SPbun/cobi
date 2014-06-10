/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   context.cpp
 * @author Jan Mussler
 * @brief Implements the context base class features
 */

#include "context.h"

#include <sstream>

using namespace std;
using namespace gi::generator;

unsigned int nextUniqueId = 1;

unsigned int getNextId() {
    return ++nextUniqueId;
}

IContext::IContext() : parent(0) {
    contextId = getNextId();
}

IContext::IContext(IContext* p) : parent(p) {
    contextId = getNextId();
}

IContext::~IContext() {
}

unsigned int IContext::getId() {
    return contextId;
}

string IContext::getStrId() {
    stringstream ss;
    ss << contextId;
    return ss.str();
}

BPatch_snippet* IContext::getLocalVariable(string name) {
    return 0;
}

BPatch_snippet* IContext::getVariable(string name) {
    if (variables.count(name) == 1) {        
        return variables[name];
    }
    else {
        BPatch_snippet* v = getLocalVariable(name);
        if (v != 0) {
            return v;
        } /*
        else if (name == "@ID@") {
            std::cout << "get IContext::ID\n";
            return new BPatch_constExpr(getStrId().c_str());
        }
        else if (name == "@INTID@") {
            return new BPatch_constExpr(static_cast<int> (getId()));
        } */
        else if (parent != 0) {
            return parent->getVariable(name);
        }
        else {
            //TODO exception, should not occure
            assert(false);
            return 0;
        }
    }
}

string IContext::getStrValue(string name) {
    if (parent != 0) {
        return parent->getStrValue(name);
    }
    else {
        assert(false);
    }
}

BPatch_function* IContext::getFunction(string name) {
    if (parent != 0) {
        return parent->getFunction(name);
    }
    else {
        return 0;
    }
}

bool IContext::createVariable(string name, string type, int size) {
    BPatch_variableExpr* var = requestVariable(type, size);
    if (var == 0) {
        return false;
    }
    variables[name] = var;
    return true;
}

BPatch_variableExpr* IContext::requestVariable(string type, int size) {
    if (parent != 0) {
        return parent->requestVariable(type, size);
    }
    else {
        return 0;
    }
}

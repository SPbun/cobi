/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   constant.cpp
 * @author Jan Mussler
 * @brief Implements the constant AST nodes
 *
 * NullOp, String and Integer const expressions
 */

#include "generator.h"

using namespace gi::generator;

BPatch_snippet* NullOp::getSnippet(IContext* c) {
    return new BPatch_nullExpr();
}

NullOp::~NullOp() {
}

NullOp::NullOp() {
}

Constant* Constant::createConstant(int i) {
    return new IntConstant(i);
}

Constant* Constant::createConstant(std::string s) {
    return new StrConstant(s);
}

IntConstant::IntConstant(int i) : value(i) {
}

string IntConstant::getStringValue(IContext* c) {
    stringstream ss;
    ss << value;
    return ss.str();
}

BPatch_snippet* IntConstant::getSnippet(IContext* c) {
    return new BPatch_constExpr(value);
}

StrConstant::StrConstant(std::string s) : value(s) {
}

string StrConstant::getStringValue(IContext* c) {
    return value;
}

BPatch_snippet* StrConstant::getSnippet(IContext* c) {
    return new BPatch_constExpr(value.c_str());
}

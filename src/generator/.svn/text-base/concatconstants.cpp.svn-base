/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   concatconstants.cpp
 * @author Jan Mussler
 * @brief Implements the concact constants operator
 */

#include "generator.h"

using namespace gi::generator;

ConcatConstants::ConcatConstants(Constant* lo, Constant* ro) : leftOp(lo),rightOp(ro),snippet(0) {

}

void ConcatConstants::setLeftOp(Constant* lo) {
    leftOp = lo;
}

void ConcatConstants::setRightOp(Constant* ro) {
    rightOp = ro;
}

string ConcatConstants::getStringValue(IContext* c) {
    return leftOp->getStringValue(c) + rightOp->getStringValue(c);
}

BPatch_snippet* ConcatConstants::getSnippet(IContext* c) {
    return new BPatch_constExpr((leftOp->getStringValue(c) + rightOp->getStringValue(c)).c_str());
}
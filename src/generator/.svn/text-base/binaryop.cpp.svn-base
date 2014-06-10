/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   main.cpp
 * @author Jan Mussler
 * @brief Implements the base class for binary operations (using two operands)
 */

#include "generator.h"

using namespace gi::generator;

BinaryOp::BinaryOp(IExpression* leftOp, IExpression* rightOp) : leftOperand(leftOp),rightOperand(rightOp) {
}

BinaryOp::~BinaryOp() {
    delete leftOperand;
    delete rightOperand;
}

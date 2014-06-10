/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   assignment.cpp
 * @author Jan Mussler
 * @brief Implements assignment AST node
 */

#include "generator.h"

namespace gi {
    namespace generator {

        Assignment::Assignment(IExpression* leftOp, IExpression* rightOp) : BinaryOp(leftOp, rightOp) {
        };

        BPatch_snippet* Assignment::getSnippet(IContext* c) {            

            BPatch_snippet* lo;
            BPatch_snippet* ro;

            lo = leftOperand->getSnippet(c);
            ro = rightOperand->getSnippet(c);

            return new BPatch_arithExpr(BPatch_assign, *lo, *ro);
        }
    }
}

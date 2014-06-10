/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   boolexpression.cpp
 * @author Jan Mussler
 * @brief Implements the boolean expression AST node
 */

#include "generator.h"

namespace gi {
    namespace generator {

        BoolExpression::BoolExpression(std::string relOp, IExpression* lOp, IExpression* rOp): BinaryOp(lOp,rOp) {
            if (relOp == "==") {
                op = EQUALS;
            }
            else if (relOp == "!=") {
                op = NOT_EQUALS;
            }
            else if (relOp == "<") {
                op = LESS;
            }
            else if (relOp == ">") {
                op = GREATER;
            }
            else if (relOp == ">=") {
                op = GREATER_EQUAL;
            }
            else if (relOp == "<=") {
                op = LESS_EQUAL;
            }
        }

        BPatch_boolExpr* BoolExpression::getBoolSnippet(IContext* c) {

            BPatch_relOp bop;
            switch (op) {
                case EQUALS: bop = BPatch_eq;
                    break;
                case NOT_EQUALS: bop = BPatch_ne;
                    break;
                case GREATER: bop = BPatch_gt;
                    break;
                case LESS: bop = BPatch_lt;
                    break;
                case LESS_EQUAL: bop = BPatch_le;
                    break;
                case GREATER_EQUAL: bop = BPatch_ge;
                    break;
                default:
                    assert(false); // should not happen, grammar
            }

            return new BPatch_boolExpr(bop,*leftOperand->getSnippet(c),*rightOperand->getSnippet(c));
        }

        BPatch_snippet* BoolExpression::getSnippet(IContext* c) {
            return getBoolSnippet(c);
        }        
    }
}

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   term.cpp
 * @author Jan Mussler
 * @brief Implementation of Term AST node
 */

#include "generator.h"

namespace gi {
    namespace generator {

        Term::Term(string binOp, IExpression* lo, IExpression* ro) : BinaryOp(lo,ro) {
            if( binOp == "+" ) {
                op = PLUS;
            }
            else if ( binOp == "-" ) {
                op = MINUS;
            }
            else if ( binOp == "*" ) {
                op = MULTIPLY;
            }
            else if ( binOp == "/" ) {
                op = DIVIDE;
            }
        }

        BPatch_snippet* Term::getSnippet(IContext* c) {
            
            BPatch_binOp bpOp;

            switch(op) {
                case PLUS: bpOp = BPatch_plus; break;
                case MINUS: bpOp = BPatch_minus; break;
                case MULTIPLY: bpOp = BPatch_times; break;
                case DIVIDE: bpOp = BPatch_divide; break;
                default: assert(false); // should not happen, grammar
            }

            return new BPatch_arithExpr(bpOp,*leftOperand->getSnippet(c),*rightOperand->getSnippet(c));
        }        
    }
}

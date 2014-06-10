/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   conditional.cpp
 * @author Jan Mussler
 * @brief Implements an If AST node
 */

#include "generator.h"

namespace gi {
    namespace generator {

        Conditional::Conditional(IBoolExpression* bExp, IExpression* tProg, IExpression* fProg) : boolExp(bExp), tProg(tProg), fProg(fProg) {

        }

        Conditional::~Conditional() {
        }

        BPatch_snippet* Conditional::getSnippet(IContext* c) {
            assert(tProg != 0);

            if (fProg == 0) {
                return new BPatch_ifExpr(*boolExp->getBoolSnippet(c), *tProg->getSnippet(c));
            }
            else {
                return new BPatch_ifExpr(*boolExp->getBoolSnippet(c), *tProg->getSnippet(c), *fProg->getSnippet(c));
            }
        }
    }
}

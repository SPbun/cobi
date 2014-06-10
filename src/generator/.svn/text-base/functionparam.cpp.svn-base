/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   functionparam.cpp
 * @author Jan Mussler
 * @brief Implements a Functionparameter Access AST node
 */

#include "generator.h"

using namespace std;

namespace gi {
    namespace generator {

        FunctionParam::FunctionParam(string aName) {
            if(aName == "@1@") {
                paramNumber = 0;
            }
            else if (aName=="@2@") {
                paramNumber = 1;
            }
            else if (aName=="@3@") {
                paramNumber = 3;
            }
            else if (aName=="@4@") {
                paramNumber = 4;
            }
            else if (aName=="@5@") {
                paramNumber = 5;
            }
            else if (aName=="@6@") {
                paramNumber = 6;
            }
            else if (aName=="@7@") {
                paramNumber = 7;
            }
            else if (aName=="@8@") {
                paramNumber = 8;
            }
            else if (aName=="@9@") {
                paramNumber = 9;
            }
            else {
                assert(false);
            }
        }

        BPatch_snippet* FunctionParam::getSnippet(IContext* c) {
            return new BPatch_paramExpr(paramNumber);
        }        
    }
}

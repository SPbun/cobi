/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   retfloatordouble.cpp
 * @author Jan Mussler
 * @brief Implementation of returns float or double property
 */

#include "implproperties.h"
#include "BPatch.h"

using namespace gi::mutatee;

namespace gi {
    namespace filter {
        namespace properties {

            RetFloatOrDouble::RetFloatOrDouble() {
            }

            RetFloatOrDouble::~RetFloatOrDouble() {
            }

            bool RetFloatOrDouble::init(Mutatee* mutatee) {
                return true;
            }

            bool RetFloatOrDouble::match(gi::mutatee::IFunction* f) {
                BPatch_type* returnType = f->getBPFunction()->getReturnType();
                string name = returnType->getName();
                
                if(name=="double" || name=="float") {
                    return true;
                }
                else {
                    return false;
                }
            }

            bool RetFloatOrDouble::needsInit() {
                return false;
            }

            bool RetFloatOrDouble::setUp(gix::FilterParser* fp, DOMElement* headElement) {
                return true;
            }
        }
    }
}

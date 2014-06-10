/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   dependency.cpp
 * @author Jan Mussler
 * @brief Implementation of Dependency class
 */

#include "dependency.h"

namespace gi {
    namespace instrumenter {

        Dependency::Dependency(string aName, string aPath) : name(aName), path(aPath) {
        }
    }
}

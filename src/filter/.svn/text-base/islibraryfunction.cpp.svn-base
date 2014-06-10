/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   islibraryfunction.cpp
 * @author Jan Mussler
 * @brief Implementation of the islibraryfunction property
 */

#include "implproperties.h"

using namespace gi::mutatee;
using namespace gi::filter::properties;
namespace gix = gi::xml;

IsLibraryFunction::IsLibraryFunction() {}

IsLibraryFunction::~IsLibraryFunction() {}

bool IsLibraryFunction::setUp(gix::FilterParser* fp, DOMElement* headElement) {
    return true;
}

bool IsLibraryFunction::init(Mutatee* mutatee) {
    return true;
}

bool IsLibraryFunction::match(gi::mutatee::IFunction* f) {
    BPatch_function* func = f->getBPFunction();
    if (func == 0) return true;
    return func->isSharedLib();
}

bool IsLibraryFunction::needsInit() {
    return false;
}

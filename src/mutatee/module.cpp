/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   module.cpp
 * @author Jan Mussler
 * @brief Module implementation
 */

#include "mutatee.h"

using namespace gi::mutatee;

Module::Module() : m(0) {
    
}

/**
 * Constructor
 * @param aM Dyninst Module reference
 **/
Module::Module(BPatch_module* aM) {
    m = aM;
}

Module::~Module() {

}

/**
 * @return name of module
 **/
std::string Module::getName() {
    assert(m!=0);
    char buffer[255];
    buffer[254] = 0;

    m->getName(buffer,254);
    
    return std::string(buffer);
}

/**
 * get functions defined in the module
 **/
std::vector<Function>* Module::getFunctions() {

    std::vector<Function>* fs = new std::vector<Function>();
    std::vector<BPatch_function*>* dynFs;

    dynFs = m->getProcedures();
    for( std::vector<BPatch_function*>::iterator i = dynFs->begin();
            i != dynFs->end();
            i++ )
    {
        Function f((*i));
        fs->push_back(f);
    }
    
    return fs;
}

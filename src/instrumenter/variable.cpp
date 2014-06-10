/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   variable.cpp
 * @author Jan Mussler
 * @brief Variable class methods
 */

#include "variable.h"

gi::instrumenter::Variable::Variable(string name, string type, unsigned int size) : name(name), type(type), size(size) {

}

bool gi::instrumenter::Variable::operator<(const gi::instrumenter::Variable& b) const {
    return name < b.name;
}

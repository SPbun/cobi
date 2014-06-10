/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   variable.h
 * @author Jan Mussler
 * @brief Variable class definitions
 */
#ifndef _VARIABLE_H
#define	_VARIABLE_H

#include <string>

namespace gi {
    namespace instrumenter {

        using namespace std;

        /**
         * Class to store a information about a Variable that will be used in the instrumentation
         */
        class Variable {
        public:
            string name;
            string type;
            unsigned int size;
            Variable(string name, string type, unsigned int size);
            bool operator<(const Variable& b) const;
        };
    }
}
#endif	/* _VARIABLE_H */

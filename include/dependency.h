/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   dependency.h
 * @author Jan Mussler
 * @brief Definition of dependency class
 *
 * simple storage class for adapter dependencies
 */

#ifndef _DEPENDENCY_H
#define	_DEPENDENCY_H

#include <string>

namespace gi {
    namespace instrumenter {

        using namespace std;

        /**
         * Class for storing a dependency that will be added to the mutatee
         */
        class Dependency {
        public:
            string name;
            string path;
            Dependency(string aName, string aPath);
        };
    }
}
#endif	/* _DEPENDENCY_H */

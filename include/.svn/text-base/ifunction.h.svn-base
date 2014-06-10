/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   ifunction.h
 * @author Jan Mussler
 * @brief Definition of IFunction interface
 */

#ifndef _IFUNCTION_H
#define	_IFUNCTION_H

#include <string>
#include "BPatch.h"

namespace gi {
    namespace mutatee {
        /**
         * Interface for function object
         */
        class IFunction {
        public:

            virtual ~IFunction() {};

            /**
             * get name of function, which is either the functionname including namespace or the mangled name
             */
            virtual std::string getName() = 0;

            /**
             * get only the name of the function, without namespace in front of it
             */
            virtual std::string getFunctionName() = 0;

            /**
             * get name of module where function is defined, in case of debug information equals source file
             */
            virtual std::string getModuleName() = 0;

            /**
             * get the compiler mangled name of the function
             */
            virtual std::string getMangledName() = 0;

            /**
             * get the class name, buest guess only, equals the closest identifier left of the functionname
             */
            virtual std::string getClassName() = 0;

            /**
             * get namespace name, guess to everything but the function name
             */
            virtual std::string getNameSpace() = 0;

            /**
             * get vector of functions which are called from this function
             * returned functions must be deletes
             * returned functions do not equal those returned from the mutatee!!
             */
            virtual std::vector<IFunction*> getSubCalls(bool incPointerCalls = false) = 0;

            virtual BPatch_function* getBPFunction() = 0;

            /**
             * @return whether function is an external function, not inside the binary
             */
            virtual bool isExternalFunc() = 0;

            /**
             * @return whether function is a functioncall made through pointer ( probably inherited function call )
             */
            virtual bool isVirtualFuncObj() = 0;
        };

    }
}

#endif	/* _IFUNCTION_H */

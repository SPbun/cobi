/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   context.h
 * @author Jan Mussler
 * @brief Context base class definitions
 */

#ifndef _CONTEXT_H
#define	_CONTEXT_H

#include <string>
#include <map>
#include "BPatch_snippet.h"

namespace gi {
    namespace generator {
        /**
         * Super class for all contexts.
         * Context store variables and possibly a parent context.
         * If variable is not found in context, search continues in parent context.
         * Idea: LoopContext -> FunctionContext -> GlobalContext
         */
        class IContext {
        private:
            unsigned int contextId;
        protected:
            unsigned int getId();
            std::string getStrId();
            /**
             * Parent context
             */
            IContext* parent;

            /**
             * Variable storage
             */
            std::map< std::string, BPatch_variableExpr* > variables;

            IContext();

            /**
             * @param p parent context
             */
            IContext(IContext* p);

            /**
             * request a variable, this will delegate the request til a context which can create it
             */
            virtual BPatch_variableExpr* requestVariable(std::string type, int size);

            /**
             * gets a local variable, local variables are those specd by @...@ in the adapter file
             * e.g., @linenumber@ @filename@ ...
             */
            virtual BPatch_snippet* getLocalVariable(std::string name);
        public:
            virtual ~IContext();

            /**
             * get a variable with given name
             */
            virtual BPatch_snippet* getVariable(std::string name);

            /**
             * create a new variable in this context with given name type and size
             */
            virtual bool createVariable(std::string name, std::string type, int size);

            /**
             * gets a dyninst function symbol for a given function name,
             * this request does check parent if symbol can not be resolved localy
             */
            virtual BPatch_function* getFunction(std::string name);

            virtual std::string getStrValue(std::string name);
        };
    }
}

#endif	/* _CONTEXT_H */

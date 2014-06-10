/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   implcontexts.h
 * @author Jan Mussler
 * @brief Defintion of implemented contexts
 */

#ifndef _ICONTEXTS_H
#define	_ICONTEXTS_H

#include "context.h"
#include "mutatee.h"
#include "instrumentations.h"
#include "adapterconfig.h"

namespace gim = gi::mutatee;

namespace gi {
    namespace generator {

        /**
         * class used for adding variables, represing context right below functioncontext
         */
        class VariableContext : public IContext {
        public:
            /**
             * constructor
             * @param p parent context
             */
            VariableContext(IContext* p, gi::instrumenter::setVariable list);
        };

        /**
         * Context Class associated with function enter and exit instrumentation points
         * should retreive local and parameter variables
         **/
        class FunctionContext : public IContext {
        private:
            BPatch_constExpr* bpName;
            BPatch_constExpr* bpFile;
            BPatch_constExpr* bpLine;
            gi::mutatee::Function* function;
            unsigned int codeStartLine;

            static int globLastFunctionID;

            BPatch_constExpr* mFuncID;

            void init();

        protected:
            virtual BPatch_snippet* getLocalVariable(string name);
        public:
            FunctionContext(IContext* p, gim::Function* f,unsigned int startLine);
            FunctionContext(gim::Function* f);
            virtual ~FunctionContext();
            string getStrValue(string name);
        };

        /**
         * Context representing a Loop
         */
        class LoopContext : public IContext {
        private:
            string loopName;
            string funcName;
            BPatch_constExpr* bpLoopName;
            BPatch_constExpr* bpFullLoopName;
        protected:
            virtual BPatch_snippet* getLocalVariable(string name);
        public:
            LoopContext(IContext* p, string name, string funcName);
            ~LoopContext();
            string getStrValue(string name);
        };

        /**
         * Context representing a CallSite
         */
        class CallSiteContext : public IContext {
        private:
            BPatch_point* point;
            BPatch_constExpr* bpCalledName;
        protected:
            /**
             * knows how to determine the called function
             */
            virtual BPatch_snippet* getLocalVariable(string name);
        public:
            CallSiteContext(IContext* p, BPatch_point* point);
            string getStrValue(string name);
        };

        /**
         * Context representing an Instrumentation Point
         */
        class InstPointContext : public IContext {
        private:
            gim::Mutatee* mutatee;
            BPatch_point* point;
        protected:
            /**
             * knows how to get the line number for the point
             */
            virtual BPatch_snippet* getLocalVariable(string name);
        public:
            string getStrValue(string name);
            InstPointContext(IContext* p, gim::Mutatee* mutatee, BPatch_point* point);
        };

        /**
         * Global Context
         *   responsible for retreiving information from the mutatee
         *   throws exceptions if variables not found
         *   represents top level context, any requests getting here must be handled
         *     or throw exception
         */
        class GlobalContext : public IContext {
        private:
            gim::Mutatee* mutatee;

            map<string, BPatch_function*> functionCache;

            /**
             * create a variable and return expressiosn
             *   all variable creation currently done here
             * @param name
             * @param type
             * @param size
             * @return dyninst variable expression
             */
            virtual BPatch_variableExpr* requestVariable(string type, int size);
        public:
            GlobalContext(gim::Mutatee* m);

            /**
             * @return associated mutatee with this global context
             */
           gim::Mutatee* getMutatee();
            /**
             * get a variable snippet associated with name
             * @param name variablename
             */
            virtual BPatch_snippet* getVariable(string name);

            /**
             * create a variable
             */
            virtual bool createVariable(string name, string type, int size);

            virtual string getStrValue(string name);

            /**
             * get function object from mutatee
             * @param name function name
             * @return dyninst function object
             */
            virtual BPatch_function* getFunction(string name);
            virtual ~GlobalContext();
        };
    }
}

#endif	/* _ICONTEXTS_H */

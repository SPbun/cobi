/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   instrumentablefunction.h
 * @author Jan Mussler
 * @brief Instrumentablefunction class defintion
 */

#ifndef _INSTRUMENTABLEFUNCTION_H
#define	_INSTRUMENTABLEFUNCTION_H

#include "mutatee.h"
#include "instrumentations.h"
#include "implcontexts.h"

using namespace std;

namespace gim = gi::mutatee;
namespace gig = gi::generator;

namespace gi {
    namespace instrumenter {

        /**
         * class for storing how and where to instrument a function
         */
        class InstrumentableFunction : public IInstrumentableFunction {
        private:
            
            Function* function;

            // loops to instrument
            loopMapType loopInstrumentations;

            // callsites to instrument
            callsiteMapType callsiteInstrumentations;

            // name of codes to instrument function with
            FunctionInstrumentation functionInstrumentations;

            typedef map<string, gig::IContext*> mapStrContext;

            /**
             * context object of this function
             */
            gig::FunctionContext* functionContext;

            /**
             * different contexts for all named codes used to instrument this function
             *     names are e.g. functions,loops,callsites or specified by the user name=""
             */
            mapStrContext contextStore;

            void deleteContextStore();
        public:
            InstrumentableFunction();
            InstrumentableFunction(Function* f);
            ~InstrumentableFunction();

            /**
             * @return Function associated with it
             */
            Function* getFunction();

            /**
             * add loop instrumentation with named code to the loop block
             */
            bool addLoopInstrumentation(BPatch_loopTreeNode* loopNode, string codeName);
            /**
             * add call site instrumentation to the callsite at point
             */
            bool addCallSiteInstrumentation(BPatch_point* point, string codeName);

            /**
             * add instrumentation with named code to this function
             */
            bool addFunctionInstrumentation(string name);

            bool isMarkedForInstrumentation();

            /**
             * store the context associated with this name, to use it later if necessary
             */
            void storeContext(string instCodeName, gig::IContext* context);

            /**
             * get the context previously stored under that name
             */
            gig::IContext* getContext(string instCodeName);

            /**
             * @return context associated with function
             */
            gig::IContext* getFunctionContext();

            /**
             * instrument this function with all the specified codes
             */
            bool instrument(gim::Mutatee* mutatee, IReadableAdapterConfig* adapter, gig::IContext* context);
        };
    }
}

#endif	/* _INSTRUMENTABLEFUNCTION_H */

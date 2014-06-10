/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   instrumentations.h
 * @author Jan Mussler
 * @brief defintion of classes related to storing how to instrument what
 */

#ifndef _INSTRUMENTATIONS_H
#define	_INSTRUMENTATIONS_H

#include <set>
#include <map>
#include <string>

#include "mutatee.h"

using namespace std;
using namespace gi::mutatee;

namespace gi {
    namespace instrumenter {

        /**
         * base class for objects to be instrumented
         * stored name represends code section in adapter spec
         */
        class InstrumentSet {
        protected:
            set<string> instrumentWith;
        public:
            /**
             * add the given name to the list, specifying that the named could shall later be used to instrument the location
             */
            bool addInstrumentation(string aCodeName);

            /**
             * returns a set of strings, nameing the codes from the adapter spec to be inserted
             */
            set<string> getSet();

            /**
             * @return true if there is at least one instrumentation
             */
            bool hasAnyInstrumentation();
        };

        /**
         * interface for instrumentable object
         */
        class IInstrumentableObject {
        public:
            virtual ~IInstrumentableObject() {};
            /**
             * instrument the object
             *   @param adapter provides the code generation
             *   @param mutatee to be instrumented
             */
            virtual void instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee) = 0;
        };

        /**
         * class storing how to instrument a function
         */
        class FunctionInstrumentation : public InstrumentSet, public IInstrumentableObject {
        private:
            IInstrumentableFunction* function;
        public:
            FunctionInstrumentation();
            FunctionInstrumentation(IInstrumentableFunction* f);
            void instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee);
        };

        /**
         * Loop that has to be instrumented
         */
        class LoopInstrumentation : public InstrumentSet, public IInstrumentableObject {
        private:
            IInstrumentableFunction* containingFunction;

            BPatch_loopTreeNode* loopNode;
            
            /**
             * instrument the body ( enter and exit of each iteration )
             */
            bool instBody;

            /**
             * instrument the loop as a whole ( before and after )
             */
            bool instLoop;
        public:
            std::string getLoopNodeName();
            LoopInstrumentation();
            LoopInstrumentation(IInstrumentableFunction* aContainingFunction, BPatch_loopTreeNode* aLoop, bool body = true, bool loop = true);
            void instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee);
        };

        /**
         * CallSite thas has to be instrumented
         */
        class CallSiteInstrumentation : public InstrumentSet, public IInstrumentableObject {
        private:
            IInstrumentableFunction* containingFunction;
            
            BPatch_point* callSite;
        public:
            CallSiteInstrumentation();
            CallSiteInstrumentation(IInstrumentableFunction* aContainingFunction, BPatch_point* aPoint);
            void instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee);
        };

        typedef map<BPatch_loopTreeNode*, LoopInstrumentation> loopMapType;
        typedef map<BPatch_point*, CallSiteInstrumentation> callsiteMapType;
    }
}

#endif	/* _INSTRUMENTATIONS_H */

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   instrument.cpp
 * @author Jan Mussler
 * @brief implementation of the loop over all instrumentable functions and
 * invokation of instrumentation per function itself
 */

#include <algorithm>

#include "adapterconfig.h"
#include "filters.h"
#include "implcontexts.h"
#include "generator.h"
#include "cobiconfig.h"

namespace gi {
    namespace instrumenter {

        bool InstrumentSet::addInstrumentation(string aCodeName) {
            if (instrumentWith.find(aCodeName) == instrumentWith.end()) {
                instrumentWith.insert(aCodeName);
                return true;
            }
            return false;
        }

        set<string> InstrumentSet::getSet() {
            return instrumentWith;
        }

        bool InstrumentSet::hasAnyInstrumentation() {
            return !instrumentWith.empty();
        }

        void instrument(Mutatee* mutatee, IReadableAdapterConfig* adapter) {
            gig::GlobalContext gc(mutatee);
            gig::IContext* globalVars = adapter->createGlobalVariableContext(&gc);

            /* add libraries to mutatee */
            vector<Dependency> dependencies = adapter->getDependencies();
            vector<Dependency>::const_iterator iterDep = dependencies.begin();
            while (iterDep != dependencies.end()) {
                mutatee->addDependency(*iterDep);
                iterDep++;
            }
            
            if(adapter->hasAdapterInitCode()) {
                mutatee->insertInitCode(adapter->getAdapterInitCode(globalVars),-1); // -1 will insert before any other init code
            }

            if(adapter->hasAdapterFinalizeCode()) {
                mutatee->insertFinalizeCode(adapter->getAdapterFinalizeCode(globalVars));
            }

            gif::filterResult* all = mutatee->getAllInstrumentableFunctions();

            vector<string> instFuncNames;
            int i = 0;

            std::cout << "\n===     instrumenting      ===\n";
            for (gi::filter::filterResult::iterator func = all->begin(); func != all->end(); func++) {
                if ((*func)->instrument(mutatee, adapter, globalVars)) {
                    instFuncNames.push_back("\t" + (*func)->getFunction()->getName() + " " + (*func)->getFunction()->getModuleName());
                    i++;
                }
            }

            if (Cobiconfig::getCfg().getFlag(Cobiconfig::LIST_INSTRUMENTED_FUNCTIONS)) {
                sort(instFuncNames.begin(), instFuncNames.end());

                for (unsigned int j = 0; j < instFuncNames.size(); j++) {
                    std::cout << "\t" << instFuncNames[j] << "\n";
                }
            }

            std::cout << "instrumented " << i << " functions\n";
            std::cout <<   "=== finished instrumenting ===\n\n";

            delete globalVars;
        }
    }
}

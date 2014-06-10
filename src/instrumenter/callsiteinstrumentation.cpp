/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   callsiteinstrumentation.cpp
 * @author Jan Mussler
 * @brief Implementation of Callsite instrumentation
 */

#include <time.h>
#include <algorithm>

#include "adapterconfig.h"
#include "filters.h"
#include "implcontexts.h"
#include "generator.h"
#include "cobiconfig.h"

namespace gii = gi::instrumenter;

using namespace gi::generator;

namespace gi {
    namespace instrumenter {

        CallSiteInstrumentation::CallSiteInstrumentation() {
        }

        CallSiteInstrumentation::CallSiteInstrumentation(IInstrumentableFunction* aContainingFunction, BPatch_point* aPoint) : containingFunction(aContainingFunction), callSite(aPoint) {
        }

        void CallSiteInstrumentation::instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee) {
            if (instrumentWith.empty()) return;

            for (set<string>::iterator codeName = instrumentWith.begin();
                    codeName != instrumentWith.end();
                    codeName++) {

                IContext* pContext = containingFunction->getContext(*codeName);

                CallSiteContext* c = new CallSiteContext(pContext, callSite);

                if (gii::getCfg().getFlag(gii::Cobiconfig::SHOW_IP_ADDRESSES)) {
                    std::cout << "[Info] Callsite Adress: " << callSite->getAddress() << "\n";
                }

                if (adapter->hasBeforeCodeFor(*codeName)) {
                    vector<BPatch_point*> points;
                    points.push_back(callSite);

                    BPatch_snippet* inst = adapter->getBeforeCodeByName(c, *codeName);
                    // set enter the call event the last to fire before the function is entered
                    mutatee->insertCode(points, inst, BPatch_callBefore, BPatch_lastSnippet);
                }

                if (adapter->hasAfterCodeFor(*codeName)) {
                    vector<BPatch_point*> points;
                    points.push_back(callSite);
                    //set to first, make call site exit the first event that fires
                    BPatch_snippet* inst = adapter->getAfterCodeByName(c, *codeName);
                    mutatee->insertCode(points, inst, BPatch_callAfter, BPatch_firstSnippet);
                }

                delete c;
            }
        }
    }
}

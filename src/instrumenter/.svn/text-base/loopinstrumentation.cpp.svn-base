/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   loopinstrumentation.cpp
 * @author Jan Mussler
 * @brief Loop instrumentation implementation
 */

#include <time.h>
#include <algorithm>

#include "adapterconfig.h"
#include "filters.h"
#include "implcontexts.h"
#include "generator.h"

using namespace gi::generator;

namespace gi {
    namespace instrumenter {

        LoopInstrumentation::LoopInstrumentation() {
        }

        LoopInstrumentation::LoopInstrumentation(IInstrumentableFunction* aContainingFunction, BPatch_loopTreeNode* aLoop, bool body, bool loop) : containingFunction(aContainingFunction), loopNode(aLoop), instBody(body), instLoop(loop) {
        }

        string LoopInstrumentation::getLoopNodeName() {
            return string(loopNode->name());
        }

        void LoopInstrumentation::instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee) {
            if (instrumentWith.size() > 0) {

                BPatch_flowGraph* controlFlow = containingFunction->getFunction()->getBPFunction()->getCFG();

                for (set<string>::iterator name = instrumentWith.begin();
                        name != instrumentWith.end();
                        name++) {
                    LoopContext* loopC = new LoopContext(containingFunction->getContext(*name), loopNode->name(), containingFunction->getFunction()->getFunctionName());

                    vector<BPatch_point*>* pointsBefore = controlFlow->findLoopInstPoints(BPatch_locLoopEntry, loopNode->loop);
                    vector<BPatch_point*>* pointsEnter = controlFlow->findLoopInstPoints(BPatch_locLoopStartIter, loopNode->loop);
                    vector<BPatch_point*>* pointsExit = controlFlow->findLoopInstPoints(BPatch_locLoopEndIter, loopNode->loop);
                    vector<BPatch_point*>* pointsAfter = controlFlow->findLoopInstPoints(BPatch_locLoopExit, loopNode->loop);

                    if (pointsBefore->size() == 0 || pointsAfter->size() == 0) {
                        std::cout << "[INFO] Loop skipped no enter or exit points found\n";
                        continue;
                    }

                    bool noIter = false;

                    if (pointsEnter->size() == 0 || pointsExit->size() == 0) {
                        noIter = true;
                    }

                    if (instLoop && adapter->hasBeforeCodeFor((*name))) {
                        // instrument loop before with code
                        
                        for (vector<BPatch_point*>::iterator pi = pointsBefore->begin(); pi != pointsBefore->end(); pi++) {

                            IContext* lpC = new InstPointContext(loopC, mutatee, *pi);
                            BPatch_snippet* beforeCode = adapter->getBeforeCodeByName(lpC, (*name));

                            vector<BPatch_point*> p;
                            p.push_back((*pi));
                            mutatee->insertCode(p, beforeCode, BPatch_callBefore, BPatch_lastSnippet);
                            delete lpC;
                        }
                    }

                    if (instBody && !noIter && adapter->hasEnterCodeFor((*name))) {                        
                        // instrument loop body entry

                        for (vector<BPatch_point*>::iterator pi = pointsEnter->begin(); pi != pointsEnter->end(); pi++) {

                            IContext* lpC = new InstPointContext(loopC, mutatee, *pi);
                            BPatch_snippet* enterCode = adapter->getEnterCodeByName(lpC, (*name));

                            vector<BPatch_point*> p;
                            p.push_back((*pi));
                            mutatee->insertCode(p, enterCode, BPatch_callBefore, BPatch_lastSnippet);
                            delete lpC;

                        }
                    }

                    if (instLoop && adapter->hasAfterCodeFor((*name))) {
                        // instrument after loop
                        
                        for (vector<BPatch_point*>::iterator pi = pointsAfter->begin(); pi != pointsAfter->end(); pi++) {                            
                            IContext* lpC = new InstPointContext(loopC, mutatee, *pi);
                            BPatch_snippet* afterCode = adapter->getAfterCodeByName(lpC, (*name));

                            vector<BPatch_point*> p;
                            p.push_back((*pi));
                            mutatee->insertCode(p, afterCode, BPatch_callBefore, BPatch_lastSnippet);
                            delete lpC;
                        }
                    }

                    if (instBody && !noIter && adapter->hasExitCodeFor((*name))) {
                        // instrument loop body exit

                        for (vector<BPatch_point*>::iterator pi = pointsExit->begin(); pi != pointsExit->end(); pi++) {                            
                            IContext* lpC = new InstPointContext(loopC, mutatee, *pi);
                            BPatch_snippet* exitCode = adapter->getExitCodeByName(lpC, (*name));

                            vector<BPatch_point*> p;
                            p.push_back((*pi));

                            mutatee->insertCode(p, exitCode, BPatch_callBefore, BPatch_firstSnippet);
                            delete lpC;
                        }
                    }

                    delete pointsBefore;
                    delete pointsEnter;
                    delete pointsExit;
                    delete pointsAfter;
                }
            }
        }
    }
}

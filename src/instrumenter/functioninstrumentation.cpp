/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   functioninstrumentation.cpp
 * @author Jan Mussler
 * @brief Instrumentation of a single function
 */

#include <time.h>
#include <algorithm>

#include "adapterconfig.h"
#include "filters.h"
#include "implcontexts.h"
#include "generator.h"
#include "cobiconfig.h"

namespace gii = gi::instrumenter;
namespace gig = gi::generator;

namespace gi {
    namespace instrumenter {

        FunctionInstrumentation::FunctionInstrumentation() {
        }

        FunctionInstrumentation::FunctionInstrumentation(IInstrumentableFunction* f) : function(f) {
        }

        void FunctionInstrumentation::instrument(IReadableAdapterConfig* adapter, Mutatee* mutatee) {
            if (instrumentWith.empty()) return;

            for (set<string>::iterator codeNameIter = instrumentWith.begin(); codeNameIter != instrumentWith.end(); codeNameIter++) {
                string codeName = (*codeNameIter);                
                gig::IContext* varContext = adapter->createVariableContextForName(function->getFunctionContext(), codeName);
                assert(varContext != 0);

                vector<BPatch_point*>* enterPoints = function->getFunction()->getBPFunction()->findPoint(BPatch_entry);
                vector<BPatch_point*>* exitPoints = function->getFunction()->getBPFunction()->findPoint(BPatch_exit);

                if (enterPoints->size() == 0
                        && false == gii::getCfg().getFlag(gii::Cobiconfig::IGNORE_NOENTRY)) {
                    std::cout << "[INFO] Function " << function->getFunction()->getName() << " was skipped, no enter points found\n";

                    delete enterPoints;
                    delete exitPoints;
                    continue;
                }

                // @todo make filter, tools other than scalasca may handle that
                // current problem is, these functions will most likely look like recursive calls
                if (exitPoints->size() == 0
                        && false == gii::getCfg().getFlag(gii::Cobiconfig::IGNORE_NOEXIT)) {
                    std::cout << "[INFO] Function " << function->getFunction()->getName() << " was skipped, no exit points found\n";

                    delete enterPoints;
                    delete exitPoints;
                    continue;
                }

                if(gii::getCfg().getFlag(gii::Cobiconfig::SHOW_IP_ADDRESSES)) {
                    std::cout << "[INFO] Function " << function->getFunction()->getName() << " InstPoint Adresses:\n";
                    std::cout << "  Entry: ";
                    bool first = true;
                    for (vector<BPatch_point*>::iterator pi = enterPoints->begin(); pi != enterPoints->end(); pi++) {
                        std::cout << (first==false?", ":"") <<(*pi)->getAddress();
                        first = false;
                    }
                    std::cout << "\n";

                    first = true;
                    std::cout << "  Exit: ";
                    for (vector<BPatch_point*>::iterator pi = exitPoints->begin(); pi != exitPoints->end(); pi++) {
                        std::cout << (first==false?", ":"") <<(*pi)->getAddress();
                        first = false;
                    }
                    
                    std::cout << "\n";
                }

                function->storeContext(codeName, varContext);

                if (adapter->hasInitCodeFor(codeName)) {
                    BPatch_snippet* initCode = adapter->getInitCodeByName(varContext, codeName);
                    if (initCode != 0) {
                        mutatee->insertInitCode(initCode,adapter->getPriorityForInitCode(codeName));
                    }
                    // do not delete initCode, because it is inserted at a later time!
                }
                
                if (adapter->hasFinalizeCodeFor(codeName)) {
                    BPatch_snippet* finalizeCode = adapter->getFinalizeCodeByName(varContext, codeName);
                    if (finalizeCode != 0) {
                        mutatee->insertFinalizeCode(finalizeCode);
                    }
                    delete finalizeCode;
                }

                if (adapter->hasEnterCodeFor(codeName)) {
                    // loop over all enter points, create a context that can provide the line number of that point
                    // and instrument the given point with the specified code

                    for (vector<BPatch_point*>::iterator pi = enterPoints->begin(); pi != enterPoints->end(); pi++) {

                        gig::IContext* con = new gig::InstPointContext(varContext, mutatee, *pi);

                        vector<BPatch_point*> points;
                        points.push_back(*pi);

                        BPatch_snippet* enterCode = adapter->getEnterCodeByName(con, codeName);

                        if (enterCode != 0) {

                            mutatee->insertCode(points, enterCode);
                        }

                        delete con;
                        delete enterCode;

                    }

                }

                if (adapter->hasExitCodeFor(codeName)) {
                    // exit code present

                    // loop over all exit points, create a context that can provide the line number of that point
                    // and instrument the given point with the specified code

                    for (vector<BPatch_point*>::iterator pi = exitPoints->begin(); pi != exitPoints->end(); pi++) {

                        gig::IContext* con = new gig::InstPointContext(varContext, mutatee, *pi);

                        vector<BPatch_point*> points;
                        points.push_back(*pi);

                        BPatch_snippet* exitCode = adapter->getExitCodeByName(con, codeName);

                        if (exitCode != 0) {
                            mutatee->insertCode(points, exitCode);
                        }

                        delete con;
                        delete exitCode;
                    }
                }


                delete enterPoints;
                delete exitPoints;
            }
        }
    }
}

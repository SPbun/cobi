/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   instrumentablefunction.cpp
 * @author Jan Mussler
 * @brief InstrumentableFunction implementation
 *
 * Implements functions responsible for instrumenting a function
 * checks whether to do function, loop and callsite instrumentation and
 * stores the context object
 */

#include "instrumentablefunction.h"
#include "cobiconfig.h"

using namespace gi::instrumenter;
using namespace gi::generator;

InstrumentableFunction::InstrumentableFunction() : function(0), functionContext(0) {

}

InstrumentableFunction::InstrumentableFunction(Function* f) : function(f), functionContext(0) {
    functionInstrumentations = FunctionInstrumentation(this);
}

InstrumentableFunction::~InstrumentableFunction() {

    deleteContextStore();

    delete functionContext;
}

Function* InstrumentableFunction::getFunction() {
    return function;
}

bool InstrumentableFunction::addFunctionInstrumentation(string name) {
    return functionInstrumentations.addInstrumentation(name);
}

bool InstrumentableFunction::addCallSiteInstrumentation(BPatch_point* point, string codeName) {
    if (callsiteInstrumentations.find(point) == callsiteInstrumentations.end()) {
        callsiteInstrumentations[point] = CallSiteInstrumentation(this, point);
    }
    return callsiteInstrumentations[point].addInstrumentation(codeName);
}

bool InstrumentableFunction::addLoopInstrumentation(BPatch_loopTreeNode* loopNode, string codeName) {
    if (loopInstrumentations.find(loopNode) == loopInstrumentations.end()) {
        loopInstrumentations[loopNode] = LoopInstrumentation(this, loopNode);
    }
    return loopInstrumentations[loopNode].addInstrumentation(codeName);
}

bool InstrumentableFunction::isMarkedForInstrumentation() {
    return functionInstrumentations.hasAnyInstrumentation()
            || !callsiteInstrumentations.empty()
            || !loopInstrumentations.empty();
}

IContext* getContextFromMapOrDefault(string name, IContext* def, map<string, IContext*>& map) {
    if (map.find(name) == map.end()) return def;
    return map[name];
}

bool InstrumentableFunction::instrument(Mutatee* mutatee, IReadableAdapterConfig* adapter, IContext* context) {
    assert(adapter != 0);
    assert(context != 0);

    if (isMarkedForInstrumentation()) {
        SourceLine sl = mutatee->getFirstSourceLineOfAddress((ulong)function->getBPFunction()->getBaseAddr());
        functionContext = new FunctionContext(context, function, sl.line);
    }
    else {
        return false;
    }

    functionInstrumentations.instrument(adapter, mutatee);

    if (!loopInstrumentations.empty()) {
        map<string,LoopInstrumentation> loops;
        for (loopMapType::iterator loop = loopInstrumentations.begin();
                loop != loopInstrumentations.end();
                loop++) {        
            loops[(*loop).second.getLoopNodeName()] = loop->second;
        }

        // sorted by name, outer loops first in order
        for(map<string,LoopInstrumentation>::iterator loop = loops.begin();
                    loop != loops.end(); loop++) {
            if ( Cobiconfig::getCfg().getFlag(Cobiconfig::LIST_INSTRUMENTED_FUNCTIONS) ) {
                std::cout << "instrumenting loop: " << loop->second.getLoopNodeName() << "\n";
            }
            loop->second.instrument(adapter,mutatee);
        }
    }

    if (!callsiteInstrumentations.empty()) {
        for (callsiteMapType::iterator csIter = callsiteInstrumentations.begin();
                csIter != callsiteInstrumentations.end();
                csIter++) {
            (*csIter).second.instrument(adapter, mutatee);
        }
    }

    return true;
}

void InstrumentableFunction::deleteContextStore() {
    for (mapStrContext::iterator i = contextStore.begin(); i != contextStore.end(); i++) {
        delete (*i).second;
        (*i).second = 0;
    }
    contextStore.clear();
}

IContext* InstrumentableFunction::getFunctionContext() {
    return functionContext;
}

void InstrumentableFunction::storeContext(string instCodeName, IContext* context) {
    // the entry should be empty - therefor no delete currently
    contextStore[instCodeName] = context;
}

IContext* InstrumentableFunction::getContext(string instCodeName) {
    mapStrContext::iterator i = contextStore.find(instCodeName);
    if (i == contextStore.end()) {
        return functionContext;
    }
    return (*i).second;
}

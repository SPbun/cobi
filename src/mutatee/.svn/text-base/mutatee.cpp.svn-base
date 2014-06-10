/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   mutatee.cpp
 * @author Jan Mussler
 * @brief  Mutatee implementation
 */

#include <set>

#include "BPatch_statement.h"

#include "mutatee.h"
#include "callgraph.h"
#include "instrumentablefunction.h"

using namespace std;
using namespace gi::mutatee;
using namespace gi::instrumenter;
using namespace gi::mutatee::graph;
using namespace gi::filter;

BPatch_point* getFirstEnterPointOfFunction(BPatch_function* func) {
    assert(func != 0);
    vector<BPatch_point*>* points = func->findPoint(BPatch_entry);

    if (points->size() > 1) {
        Function f(func);
        std::cout << "[Warning]: "
                << f.getName() << " has multiplie entry points, \"Init\" code placed at 1st! only\n";
    }
    else if (points->size() == 0) {
        Function f(func);
        std::cout << "[Error]: "
                << f.getName() << " has no entry points, \"Init\" code could not be inserted!\n";
        exit(EXIT_FAILURE);
    }

    BPatch_point* p = (*points)[0];

    delete points;

    return p;
}

Mutatee::Mutatee() : baseFunctions(0), baseSet(0), callGraph(0), initInsertPoint(0), finalizeInsertPoints(0) {
    adapterRule = new RFalse();
}

Mutatee::Mutatee(BPatch_binaryEdit* as, BPatch_image* im) : addressSpace(as), image(im), baseFunctions(0), baseSet(0), callGraph(0), initInsertPoint(0), finalizeInsertPoints(0) {
    adapterRule = new RFalse();
}

Mutatee::~Mutatee() {

    for (set<IInstrumentableFunction*>::iterator i = baseSet->begin(); i != baseSet->end(); i++) {
        delete (*i);
    }
    baseSet->clear();

    delete finalizeInsertPoints;
    delete callGraph;
    delete baseSet;
    delete baseFunctions;
    delete addressSpace;
    delete adapterRule;
}

void Mutatee::setAdapterRule(IRule* rule) {
    delete adapterRule;
    if (rule == 0) {
        adapterRule = new RFalse();
        return;
    }
    adapterRule = rule;
}

vector<Module>* Mutatee::getModules() {

    vector<Module>* ms = new vector<Module > ();

    vector<BPatch_module*>* dynModules = image->getModules();

    for (vector<BPatch_module*>::iterator dynMod = dynModules->begin();
            dynMod != dynModules->end();
            dynMod++) {
        ms->push_back(Module(*dynMod));
    }

    return ms;
}

vector<Function*>* Mutatee::getAllFunctions() {
    if (baseFunctions != 0) return baseFunctions;

    baseFunctions = new vector<Function*> ();
    vector<BPatch_function*>* dynFunctions = image->getProcedures();

    for (vector<BPatch_function*>::iterator dynFunc = dynFunctions->begin();
            dynFunc != dynFunctions->end();
            dynFunc++) {
        baseFunctions->push_back(new Function((*dynFunc)));
    }
    delete dynFunctions;

    return baseFunctions;
}

set<IInstrumentableFunction*>* Mutatee::getAllInstrumentableFunctions() {
    if (baseSet != 0) return baseSet;

    assert(adapterRule != 0);

    std::cout << "=== requesting all instrumentable functions ===\n";

    baseSet = new set<IInstrumentableFunction*>();

    vector<BPatch_function*>* dynFunctions = image->getProcedures(false);

    std::cout << "number of functions in mutatee: " << dynFunctions->size() << "\n";

    for (vector<BPatch_function*>::iterator dynFunc = dynFunctions->begin();
            dynFunc != dynFunctions->end();
            dynFunc++) {

        Function* function = new Function((*dynFunc));

        /**
         * adapter rule excludes all functions according to the adapte filter defined adapter spec file
         */
        if (!adapterRule->match(function)) {
            InstrumentableFunction* insF = new InstrumentableFunction(function);
            baseSet->insert(insF);
        }
        else {
            delete function;
        }
    }
    std::cout << "          after adapter filter: " << baseSet->size() << "\n";

    delete dynFunctions;
    std::cout << "===============================================\n\n";

    return baseSet;
}

BPatch_function* Mutatee::getFunction(std::string name) {
    vector<BPatch_function*> fs;
    image->findFunction(name.c_str(), fs);
    if (fs.size() > 1) {
        std::cout << "warn: more than one function with name " << name << " found ( using 1st one )\n";
    }
    else if (fs.size() == 0) {
        return 0;
    }
    return fs[0];
}

void Mutatee::insertCode(vector<BPatch_point*> points, BPatch_snippet* snippet) {
    addressSpace->insertSnippet(*snippet, points);
}

void Mutatee::insertCode(vector<BPatch_point*> points, BPatch_snippet* snippet, BPatch_callWhen w, BPatch_snippetOrder o) {
    addressSpace->insertSnippet(*snippet, points, w, o);
}

bool Mutatee::save(string fileName) {
    assert(initFunction != 0);

    if (initInsertPoint == 0) {
        initInsertPoint = getFirstEnterPointOfFunction(initFunction);
    }

    assert(initInsertPoint != 0);

    vector<BPatch_point*> points;
    points.push_back(initInsertPoint);

    /*
     Insert intialization code in reverser order to produce correct result
     need first snippet, as point may be instrumented by "enter" code, which must run after init code
     */

    for (mapPrioToSnippets::reverse_iterator i = initSnippets.rbegin();
            i != initSnippets.rend(); i++) {

        for (vector<BPatch_snippet*>::reverse_iterator j = (*i).second.rbegin();
                j != (*i).second.rend(); j++) {
            insertCode(points, (*j), BPatch_callBefore, BPatch_firstSnippet);            
        }
        
    }

    return addressSpace->writeFile(fileName.c_str());
}

SourceLine::SourceLine(int l, string f) : line(l), file(f) {

}

SourceLine Mutatee::getFirstSourceLineOfAddress(unsigned long address) {
    vector<BPatch_statement> statements;

    if (addressSpace->getSourceLines(address, statements)) {
        if (!statements.size() > 0) {
            return SourceLine(0, "unknown");
        }
        return SourceLine(statements[0].lineNumber(), statements[0].fileName());
    }
    else {
        return SourceLine(0, "unknown - (getSourceLines returned false)");
    }
}

CallGraph* getCallGraphForMutatee(Mutatee* m) {
    CallGraph* gr = new CallGraph();

    // do not delete allFs!! mutatee takes care of that, and will allways return the same vector!!
    vector<Function*>* allFs = m->getAllFunctions();

    for (vector<Function*>::iterator i = allFs->begin();
            i != allFs->end();
            i++) {
        gr->addFunction(*i);
    }

    return gr;
}

graph::CallGraph& Mutatee::getCallGraph() {

    if (callGraph == 0) {

        callGraph = getCallGraphForMutatee(this);
        callGraph->transformTmpCalls();

    }

    return *callGraph;
}

BPatch_variableExpr* Mutatee::createVariable(int size) {
    return addressSpace->malloc(size);
}

BPatch_variableExpr* Mutatee::createVariable(string typeName) {
    BPatch_type* type = image->findType(typeName.c_str());
    if (type == 0) {
        return 0;
    }
    return addressSpace->malloc(*type);
}

void Mutatee::insertInitCode(BPatch_snippet* snippet, int prio) {
    insertInitCode(snippet, BPatch_callBefore, BPatch_firstSnippet, prio);
}

/**
 *
 * order of snippets ignored, use priority and inverse order in save with firstsnippet to insert correctly
 *
 * @param snippet snippet to insert
 * @param w
 * @param o
 * @param prio priority of snippet according to other init codes, smaller means executed first
 */
void Mutatee::insertInitCode(BPatch_snippet* snippet, BPatch_callWhen w, BPatch_snippetOrder o, int prio) {
    assert(initFunction != 0);
    assert(snippet!=0);
    
    initSnippets[prio].push_back(snippet);
}

void Mutatee::insertFinalizeCode(BPatch_snippet* snippet) {
    // TODO finalize code support priority, too?
    insertFinalizeCode(snippet, BPatch_callAfter, BPatch_lastSnippet);
}

void Mutatee::insertFinalizeCode(BPatch_snippet* snippet, BPatch_callWhen w, BPatch_snippetOrder o) {
    assert(initFunction != 0);

    if (finalizeInsertPoints == 0) {
        finalizeInsertPoints = finalizeFunction->findPoint(BPatch_exit);
    }

    insertCode(*finalizeInsertPoints, snippet, w, o);
}

void Mutatee::setNameOfFunctionWhereToPlaceInit(string name) {
    initFunction = getFunction(name);

    assert(initFunction != 0);
}

void Mutatee::setNameOfFunctionWhereToPlaceFinalize(string name) {
    finalizeFunction = getFunction(name);

    assert(finalizeFunction != 0);
}

void Mutatee::addDependency(gii::Dependency dep) {
    if (!addressSpace->loadLibrary((dep.path + dep.name).c_str(), false)) {
        std::cout << "[WARNING] could not load library!\n";
    }
}

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   adapterconfig.cpp
 * @author Jan Mussler
 * @brief Implementation of Adapterconfig class
 */

#include "adapterconfig.h"
#include "implcontexts.h"

using namespace std;
using namespace gi::generator;
using namespace gi::instrumenter;

AdapterConfig::AdapterConfig() : adapterFilterRule(0), mSaveAllFPRs(false), mFunctionForInit(""), mFunctionForFinalize(""), mAdapterInitCode(0), mAdapterFinalizeCode(0) {
}

AdapterConfig::~AdapterConfig() {
    delete adapterFilterRule;
}

IContext* AdapterConfig::createVariableContextForName(IContext* parentContext, string name) {
    assert(parentContext != 0);

    if (varListPerName.find(name) != varListPerName.end()) {
        return new VariableContext(parentContext, varListPerName[name]);
    }

    return new VariableContext(parentContext, setVariable()); // return empty context, makes cleanup easier
}

IContext* AdapterConfig::createGlobalVariableContext(IContext* parentContext) {
    assert(parentContext != 0);

    if (!globalVariables.empty()) {
        return new VariableContext(parentContext, globalVariables);
    }

    return new VariableContext(parentContext, setVariable()); // return empty context, makes cleanup easier
}

BPatch_snippet* AdapterConfig::getInitCodeByName(IContext* context, string name) {
    if (initCode.find(name) == initCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return initCode[name]->getSnippet(context);
}

int AdapterConfig::getPriorityForInitCode(std::string name) {
    if (initPriorities.find(name) == initPriorities.end()) {
        assert(false);
    }

    return initPriorities[name];
}

BPatch_snippet* AdapterConfig::getFinalizeCodeByName(IContext* context, string name) {
    if (finalizeCode.find(name) == finalizeCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return finalizeCode[name]->getSnippet(context);
}

BPatch_snippet* AdapterConfig::getBeforeCodeByName(IContext* context, string name) {
    if (beforeCode.find(name) == beforeCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return beforeCode[name]->getSnippet(context);
}

BPatch_snippet* AdapterConfig::getAfterCodeByName(IContext* context, string name) {
    if (afterCode.find(name) == afterCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return afterCode[name]->getSnippet(context);
}

BPatch_snippet* AdapterConfig::getEnterCodeByName(IContext* context, string name) {
    if (enterCode.find(name) == enterCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return enterCode[name]->getSnippet(context);
}

BPatch_snippet* AdapterConfig::getExitCodeByName(IContext* context, string name) {
    if (exitCode.find(name) == exitCode.end()) {
        throw ex::CodeNotDefindedException();
    }
    return exitCode[name]->getSnippet(context);
}

bool AdapterConfig::hasInitCodeFor(string name) {
    return initCode.find(name) != initCode.end();
}

bool AdapterConfig::hasFinalizeCodeFor(string name) {
    return finalizeCode.find(name) != finalizeCode.end();
}

bool AdapterConfig::hasExitCodeFor(string name) {
    return exitCode.find(name) != exitCode.end();
}

bool AdapterConfig::hasEnterCodeFor(string name) {
    return enterCode.find(name) != enterCode.end();
}

bool AdapterConfig::hasBeforeCodeFor(string name) {
    return beforeCode.find(name) != beforeCode.end();
}

bool AdapterConfig::hasAfterCodeFor(string name) {
    return afterCode.find(name) != afterCode.end();
}

void AdapterConfig::addInitExpression(IExpression* e, string name, int prio) {
    if (e != 0) {
        initCode[name] = e;
        initPriorities[name] = prio;
    }
}

void AdapterConfig::addFinalizeExpression(IExpression* e, string name) {
    if (e != 0) {
        finalizeCode[name] = e;
    }
}

void AdapterConfig::addBeforeExpression(IExpression* e, string name) {
    if (e != 0) {
        beforeCode[name] = e;
    }
}

void AdapterConfig::addAfterExpression(IExpression* e, string name) {
    if (e != 0) {
        afterCode[name] = e;
    }
}

void AdapterConfig::addEnterExpression(IExpression* e, string name) {
    if (e != 0) {
        enterCode[name] = e;
    }
}

void AdapterConfig::addExitExpression(IExpression* e, string name) {
    if (e != 0) {
        exitCode[name] = e;
    }
}

void AdapterConfig::addVariableList(set<Variable> list, string name) {
    varListPerName[name] = list;
}

void AdapterConfig::setGlobalVariableList(setVariable list) {
    globalVariables = list;
}

bool AdapterConfig::hasGlobalVariables() {
    return globalVariables.empty();
}

void AdapterConfig::addDependency(Dependency dep) {
    depList.push_back(dep);
}

vector<Dependency> AdapterConfig::getDependencies() {
    return depList;
}

void AdapterConfig::setAdapterFilterRule(gi::filter::IRule* rule) {
    adapterFilterRule = rule;
}

gi::filter::IRule* AdapterConfig::getAdapterFilterRule() {
    return adapterFilterRule;
}

bool AdapterConfig::hasAdapterFilterRule() {
    return adapterFilterRule != 0;
}

void AdapterConfig::addNonReturningFunction(std::string funcName) {
    nonReturningFunctions.insert(funcName);
}

std::set<std::string> AdapterConfig::getNonReturningFunctions() {
    return nonReturningFunctions;
}

void AdapterConfig::setSaveAllFPRs(bool value) {
    mSaveAllFPRs = value;
}

bool AdapterConfig::saveAllFPRs() {
    return mSaveAllFPRs;
}

void AdapterConfig::setFunctionForInitCode(string name) {
    mFunctionForInit = name;
}

string AdapterConfig::getFunctionForInitCode() {
    return mFunctionForInit;
}

void AdapterConfig::setFunctionForFinalize(string name) {
    mFunctionForFinalize = name;
}

string AdapterConfig::getFunctionForFinalizeCode() {
    return mFunctionForFinalize;
}

bool AdapterConfig::hasAdapterInitCode() {
    return mAdapterInitCode != 0;
}

bool AdapterConfig::hasAdapterFinalizeCode() {
    return mAdapterFinalizeCode != 0;
}

void AdapterConfig::setAdapterFinalizeCode(gig::IExpression* code) {
    mAdapterFinalizeCode = code;
}

void AdapterConfig::setAdapterInitCode(gig::IExpression* code) {
    mAdapterInitCode = code;
}

BPatch_snippet* AdapterConfig::getAdapterInitCode(IContext* context) {
    if (mAdapterInitCode != 0)
        return mAdapterInitCode->getSnippet(context);
    else
        return 0;
}

BPatch_snippet* AdapterConfig::getAdapterFinalizeCode(IContext* context) {
    if (mAdapterFinalizeCode != 0)
        return mAdapterFinalizeCode->getSnippet(context);
    else
        return 0;
}
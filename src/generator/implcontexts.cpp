/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   implcontexts.cpp
 * @author Jan Mussler
 * @brief Implementation of the various contexts
 *
 * Implementations of the different contexts including the functions to
 * resolve the context access variables enclosed with @ @
 */

#include "generator.h"
#include "implcontexts.h"
#include "mutatee.h"
#include "idregister.h"

using namespace gi::generator;
using namespace gi::mutatee;

namespace gii = gi::instrumenter;

int FunctionContext::globLastFunctionID = 0;

VariableContext::VariableContext(IContext* p, gi::instrumenter::setVariable list) : IContext(p) {
    for (gii::setVariable::iterator i = list.begin();
            i != list.end();
            i++) {
        BPatch_variableExpr* v = requestVariable((*i).type, (*i).size);
        if (v != 0) {
            variables[(*i).name] = v;
        }
    }
}

void FunctionContext::init() {
    bpFile = 0;
    bpName = 0;
    bpLine = 0;
    mFuncID = 0;
}

FunctionContext::FunctionContext(Function* f) : function(f) {
    init();
}

FunctionContext::FunctionContext(IContext* p, Function* f, unsigned int startLine) : IContext(p), function(f), codeStartLine(startLine) {
    init();
}

BPatch_snippet* FunctionContext::getLocalVariable(string name) {    
    if (name.substr(0, 1) == "@") {
        string id = name.substr(1, name.length() - 2);

        if (id == "ID") {
            gii::IdRegister::get().addId(gii::IdRegister::FUNCTION, getId(), function->getName());
            return new BPatch_constExpr(getStrId().c_str());
        }
        else if (id == "INTID") {
            gii::IdRegister::get().addId(gii::IdRegister::FUNCTION, getId(), function->getName());
            return new BPatch_constExpr(static_cast<int> (getId()));
        }
        else if (id == "ROUTINEID") {
            if (mFuncID == 0) {
                mFuncID = new BPatch_constExpr(globLastFunctionID);
                globLastFunctionID++;
            }
            return mFuncID;
        }
        else if (id == "ROUTINE") {
            if (bpName == 0) {
                bpName = new BPatch_constExpr(function->getName().c_str());
            }
            return bpName;
        }
        else if (id == "FILE") {
            if (bpFile == 0) {
                bpFile = new BPatch_constExpr(function->getModuleName().c_str());
            }
            return bpFile;
        }
        else if (id == "LINE") {
            if (bpLine == 0) {
                bpLine = new BPatch_constExpr(codeStartLine);
            }
            return bpLine;
        }
    }
    return 0;
}

string FunctionContext::getStrValue(string name) {
    if (name == "@ROUTINE@") {
        return function->getName();
    }
    else if (name == "@FILE@") {
        return function->getModuleName();
    }
    else if (name == "@LINE@") {
        stringstream ss;
        ss << codeStartLine;
        return ss.str();
    }
    else if (name == "@ID@") {
        return getStrId();
    }
    else {
        return parent->getStrValue(name);
    }
}

FunctionContext::~FunctionContext() {
    delete bpLine;
    delete bpName;
    delete bpFile;
};

LoopContext::LoopContext(IContext* p, string name, string funcName) : IContext(p), loopName(name), funcName(funcName), bpLoopName(0), bpFullLoopName(0) {

}

BPatch_snippet* LoopContext::getLocalVariable(string name) {
    if (name.substr(0, 1) == "@") {
        string id = name.substr(1, name.length() - 2);
        if (id == "ID") {
            gii::IdRegister::get().addId(gii::IdRegister::LOOP, getId(), getStrValue("@ROUTINE@") + "->" + loopName);
            return new BPatch_constExpr(getStrId().c_str());
        }
        else if (id == "INTID") {
            gii::IdRegister::get().addId(gii::IdRegister::LOOP, getId(), getStrValue("@ROUTINE@") + "->" + loopName);
            return new BPatch_constExpr(static_cast<int> (getId()));
        }
        else if (id == "LOOP") {
            if (bpLoopName == 0) {
                bpLoopName = new BPatch_constExpr(loopName.c_str());
            }
            return bpLoopName;
        }
        else if (id == "FULLLOOP") {
            if (bpFullLoopName == 0) {
                string s = funcName + " " + loopName;
                bpFullLoopName = new BPatch_constExpr(s.c_str());
            }
            return bpFullLoopName;
        }
    }
    return 0;
}

string LoopContext::getStrValue(string name) {
    if (name == "@LOOP@") {
        return loopName;
    }
    else if (name == "@FULLLOOP@") {
        return funcName + "_" + loopName;
    }
    else if (name == "@ID@") {
        return getStrId();
    }
    else {
        assert(parent != 0);
        return parent->getStrValue(name);
    }
}

LoopContext::~LoopContext() {
    delete bpLoopName;
    delete bpFullLoopName;
}

GlobalContext::~GlobalContext() {

}

GlobalContext::GlobalContext(gi::mutatee::Mutatee* m) : mutatee(m) {

}

gi::mutatee::Mutatee* GlobalContext::getMutatee() {
    return mutatee;
}

BPatch_snippet* GlobalContext::getVariable(string name) {
    throw ex::UnknownVariable();
}

string GlobalContext::getStrValue(string name) {
    throw ex::UnknownVariable();
}

bool GlobalContext::createVariable(string name, string type, int size) {
    return false;
}

BPatch_function* GlobalContext::getFunction(string name) {
    if (functionCache.find(name) == functionCache.end()) {
        BPatch_function* func = mutatee->getFunction(name);
        if (func == 0) {
            throw ex::UnknownFunction();
        }
        functionCache[name] = func;
    }
    return functionCache[name];
}

BPatch_variableExpr* GlobalContext::requestVariable(string type, int size) {
    BPatch_variableExpr* var = mutatee->createVariable(type);
    if (var == 0) {
        std::cout << "[INFO]: could not resolve type " << type << " using " << size << " bytes \n";
        var = mutatee->createVariable(size);
    }
    return var;
}

CallSiteContext::CallSiteContext(IContext* p, BPatch_point* point) : IContext(p), point(point), bpCalledName(0) {
}

BPatch_snippet* CallSiteContext::getLocalVariable(string name) {
    if (name.substr(0, 1) == "@") {
        string id = name.substr(1, name.length() - 2);

        if (id == "ID") {
            BPatch_function* bpf = point->getCalledFunction();
            Function f(bpf);
            gii::IdRegister::get().addId(gii::IdRegister::CALLSITE, getId(), getStrValue("@ROUTINE@") + "->" + f.getFunctionName());
            return new BPatch_constExpr(getStrId().c_str());
        }
        else if (id == "INTID") {
            BPatch_function* bpf = point->getCalledFunction();
            Function f(bpf);
            gii::IdRegister::get().addId(gii::IdRegister::CALLSITE, getId(), getStrValue("@ROUTINE@") + "->" + f.getFunctionName());
            return new BPatch_constExpr(static_cast<int> (getId()));
        }
        else if (id == "CALLEDROUTINE") {
            if (bpCalledName == 0) {
                BPatch_function* bpf = point->getCalledFunction();
                if (bpf != 0) {
                    Function f(bpf);
                    bpCalledName = new BPatch_constExpr(f.getFunctionName().c_str());
                }
                else {
                    bpCalledName = new BPatch_constExpr(point->getCalledFunctionName().c_str());
                }
            }
            return bpCalledName;
        }
    }
    return 0;
}

string CallSiteContext::getStrValue(string name) {
    if (name == "@ID@") {
        return getStrId();
    }
    else if(name=="@CALLEDROUTINE@") {
        BPatch_function* bpf = point->getCalledFunction();
        Function f(bpf);
        return f.getFunctionName();
    }
    else {
        assert(parent != 0);
        return parent->getStrValue(name);
    }
}

InstPointContext::InstPointContext(IContext* p, gim::Mutatee* mutatee, BPatch_point* point) : IContext(p), mutatee(mutatee), point(point) {

}

BPatch_snippet* InstPointContext::getLocalVariable(string name) {
    if (name.substr(0, 1) == "@") {
        string id = name.substr(1, name.length() - 2);
        if (id == "LINE") {
            return new BPatch_constExpr(mutatee->getFirstSourceLineOfAddress((ulong) point->getAddress()).line);
        }
        else if (id == "ADDRESS") {
            return new BPatch_constExpr((ulong) point->getAddress());
        }
    }
    return 0;
}

string InstPointContext::getStrValue(string name) {
    if (name == "@LINE@") {
        stringstream ss;
        ss << mutatee->getFirstSourceLineOfAddress((ulong) point->getAddress()).line;
        return ss.str();
    }
    else if (name == "@ADDRESS@") {
        // std::cout << "ADR: " << hex << (ulong) point->getAddress() << "\n";
        stringstream ss;
        ss << hex << (ulong) point->getAddress();
        return ss.str();
    }
    else {
        return parent->getStrValue(name);
    }
}

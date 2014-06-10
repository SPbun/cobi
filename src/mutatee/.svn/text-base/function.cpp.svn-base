/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   function.cpp
 * @author Jan Mussler
 * @brief  Function implementation
 */

#include "mutatee.h"
#include <boost/tokenizer.hpp>

using namespace gi::mutatee;
using namespace std;

// Buffer Size für MangledName und Name von Dyninst ( demangled namse insbesondere von std::map werden noch länger )
const int BUFFER_SIZE = 500;

/**
 * sperate suffix of string taking into account < > templates
 *
 * @param input
 * @return position of last "::" seperating namespace/classname or classname/function
 */
size_t getTemplatePosition(const string& input) {
    if(input.size() > 12 && input.substr(input.size()-12) == "::operator>>") {
        return input.size()-11;
    }
    if(input.find_last_of(">") <= input.find_last_of("::")) {
        return string::npos;
    }

    // go back to closing "<"

    int i = 1;
    int pos = input.find_last_of(">");
    while( i > 0 && pos > 0) {
        --pos;
        if(input.at(pos) =='<') {
            --i;
        }
        else if(input.at(pos)=='>') {
            ++i;
        }
    }
    return pos;
}

size_t findLastSeperator(string input) {
    size_t pos = input.find_last_of("::", getTemplatePosition(input));
    if(pos!=string::npos) return pos+1;
    else return string::npos;
}

IFunction* gi::mutatee::getFunctionToPoint(BPatch_point* p) {
    if (p->isDynamic()) {
        return new ExternalFunction("[DYNAMIC CALLSITE]");
    }
    if (p->getCalledFunction() == 0) {
        return new ExternalFunction(p->getCalledFunctionName());
    }
    return new Function(p->getCalledFunction());
}

/**
 * set function name
 * probably mangled name to external function
 */
ExternalFunction::ExternalFunction(string aName) : name(aName) {
}

ExternalFunction::~ExternalFunction() {
}

/**
 * get mangled name
 * equals getName for external function
 */
string ExternalFunction::getMangledName() {
    return name;
}

/**
 * get mangled name
 * equals getName for external function
 * @return name of function
 */
string ExternalFunction::getName() {
    return name;
}

string ExternalFunction::getClassName() {
    return "";
}

string ExternalFunction::getNameSpace() {
    return "";
}

string ExternalFunction::getFunctionName() {
    return name;
}

string ExternalFunction::getModuleName() {
    return "";
}

/**
 * get calls within function
 * return empty vector, since external function can not be inspected
 * @return empty vector
 */
vector<IFunction*> ExternalFunction::getSubCalls(bool incPointerCalls) {
    vector<IFunction*> fs;
    return fs;
}

BPatch_function* ExternalFunction::getBPFunction() {
    return 0;
}

bool ExternalFunction::isExternalFunc() {
    return true;
}

bool ExternalFunction::isVirtualFuncObj() {
    return false;
}

/**
 * @return compiler mangled name of function
 */
string Function::getMangledName() {
    char buffer[BUFFER_SIZE];
    buffer[BUFFER_SIZE-1] = 0;
    f->getMangledName(buffer, BUFFER_SIZE-1);
    return string(buffer);
}

/**
 * @return vector of called functions
 */
vector<IFunction*> Function::getSubCalls(bool incPointerCalls) {
    vector<IFunction*> fs;

    vector<BPatch_point*>* subR = f->findPoint(BPatch_subroutine);
    vector<BPatch_point*>::const_iterator i = subR->begin();

    for (; i != subR->end(); i++) {
        BPatch_function* calledFunction = (*i)->getCalledFunction();
        if (calledFunction == 0 && (*i)->getCalledFunctionName() != "") {
            ExternalFunction* e = new ExternalFunction((*i)->getCalledFunctionName());
            fs.push_back(e);
        }
        else if (calledFunction != 0) {
            Function* called = new Function(calledFunction);
            fs.push_back(called);
        }
        else if(incPointerCalls) {
            VirtualFunction* called = new VirtualFunction();
            fs.push_back(called);
        }
    }
    delete subR;

    return fs;
}

/**
 * create Function object for dyninst function
 */
Function::Function(BPatch_function* aF) {
    f = aF;
    name = "";
    functionname = "";
}

/**
 * get dyninst internal function reference
 * @return dyninst function reference
 */
BPatch_function* Function::getBPFunction() {
    return f;
}

/**
 * @return complete name of function
 */
string Function::getName() {
    if (name != "") return name;
    char buffer[BUFFER_SIZE];
    buffer[BUFFER_SIZE-1] = 0;

    assert(f != 0);

    f->getName(buffer, BUFFER_SIZE-1);
    
    name = string(buffer);
    return name;
}

/**
 * @return part of complete name believed to be the function identifier
 * */
string Function::getFunctionName() {
    if (functionname != "") return functionname;
    string full = getName();
    size_t pos = findLastSeperator(full);

    if (pos != string::npos) {
        functionname = full.substr(pos);
    }
    else {
        functionname = full;
    }
    
    return functionname;
}

string Function::getModuleName() {
    char buffer[BUFFER_SIZE];
    buffer[BUFFER_SIZE-1] = 0;
    f->getModuleName(buffer, BUFFER_SIZE-1);
    return string(buffer);
}

string Function::getClassName() {
    string n = getNameSpace();
    size_t pos = findLastSeperator(n);
    if (pos != string::npos) {
        return n.substr(pos);
    }
    else {
        return n;
    }
}

string Function::getNameSpace() {
    string full = getName();
    size_t pos = findLastSeperator(full);

    if ( pos != string::npos) {
        return full.substr(0, pos-2);
    }
    else {
        return "";
    }
}

bool Function::isExternalFunc() {
    return false;
}

bool Function::isVirtualFuncObj() {
    return false;
}

Function::~Function() {

}

Parameter::Parameter(string aName) {
    name = aName;
}

string Parameter::getName() {
    return name;
}

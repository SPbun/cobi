/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filter.cpp
 * @author Jan Mussler
 * @brief Filter class implementation
 */

#include <string>
#include <stack>
#include <boost/tokenizer.hpp>

#include "filters.h"

using namespace gi::filter;
using namespace std;

Filter::Filter(string aName, string startsetName, string instrument, IFilterSet* parent) : name(aName), startsetname(startsetName), instSettings(instrument), parentFilterSet(parent), callSiteRule(0), loopRule(0) {
    /**
     * initialize with Null-Object Pattern - no check for pointer = null
     */
    callSiteRule = new RFalse();
    loopRule = new RFalse();
    description = "";
    loopsOnlyOuter = false;
}

bool Filter::isInstrumenting() {
    return instrumentFunctions() || instrumentCallSites() || instrumentLoops();
}

Filter::~Filter() {
    for (vector<IFilterElement*>::iterator i = filters.begin();
            i != filters.end();
            i++) {
        delete (*i);
    }
    filters.clear();

    delete callSiteRule;
    delete loopRule;
}

bool Filter::instrumentFunctions() {
    return getFunctionCodeName() != "";
}

bool Filter::instrumentLoops() {
    return getLoopCodeName() != "";
}

bool Filter::instrumentCallSites() {
    return getCallSiteCodeName() != "";
}

string Filter::getFunctionCodeName() {
    return instSettings.functions;
}

string Filter::getLoopCodeName() {
    return instSettings.loops;
}

string Filter::getCallSiteCodeName() {
    return instSettings.callsites;
}

bool Filter::isReferenceAble() {
    return filters.size() == 1;
}

std::string Filter::getName() {
    return name;
}

std::string Filter::getDescription() {
    return description;
}

void Filter::setDescription(string desc) {
    description = desc;
}

void Filter::setLoopsOnlyOuter(bool value) {
    loopsOnlyOuter = value;
}

void Filter::addElement(IFilterElement* newElement) {
    filters.push_back(newElement);
}

void Filter::setLoopRule(IRule* rule) {
    delete(loopRule);
    if (rule != 0)
        loopRule = rule;
    else
        loopRule = new RFalse();
}

void Filter::setCallSiteRule(IRule* rule) {
    delete(callSiteRule);
    if (rule != 0) {
        callSiteRule = rule;
    }
    else {
        callSiteRule = new RFalse();
    }
}

void Filter::addLoopName(string name) {
    loopNames.insert(name);
}

bool nameMatch(const string& name, const string& pattern) {
    string::size_type pos = pattern.find("*");
    if( pos == std::string::npos) {
        return pattern==name;
    }
    else {
        return pattern.substr(0,pos) == name.substr(0,pos);
    }
}

bool Filter::checkLoopByName(string name) {

    // no filter set thus all loops
    if(loopNames.empty())
        return true;

    for (set<string>::const_iterator i = loopNames.begin(); i != loopNames.end(); i++) {
        if (nameMatch(name, *i)) {
            return true;
        }
    }
    return false;
}

filterResult* Filter::getResult(filterResult* baseSet) {
    filterResult* current;

    std::cout << "filter \"" << name << "\":\n";
    if (description != "") {
        std::cout << description << "\n";
    }

    if (startsetname == "all") {
        current = new filterResult(baseSet->begin(), baseSet->end());
    }
    else {
        current = new filterResult();
    }

    // generate set of functions to be instrumented by this filter
    for (vector<IFilterElement*>::iterator i = filters.begin();
            i != filters.end();
            i++
            ) {
        std::cout << "\tnew element\n";
        filterResult* newSet = (*i)->getResult(baseSet, current);
        delete current;
        current = newSet;
    }

    // current now contains all functions to be instrumented by the code specified
    if (instrumentFunctions()) {
        // add the named code to all functions in the set
        for (filterResult::iterator f = current->begin();
                f != current->end(); f++) {
            (*f)->addFunctionInstrumentation(getFunctionCodeName());
        }
    }

    if (instrumentCallSites()) {
        for (filterResult::iterator f = current->begin();
                f != current->end(); f++) {

            BPatch_function* bpFunc = (*f)->getFunction()->getBPFunction();

            vector<BPatch_point*>* callSites = bpFunc->findPoint(BPatch_subroutine);

            for (vector<BPatch_point*>::iterator point = callSites->begin();
                    point != callSites->end(); point++) {

                IFunction* calledF = mutatee::getFunctionToPoint(*point);

                if (!callSiteRule->match(calledF)) continue;

                (*f)->addCallSiteInstrumentation((*point), getCallSiteCodeName());
            }
        }
    }

    if (instrumentLoops()) {
        for (filterResult::iterator f = current->begin();
                f != current->end(); f++) {

            if (!loopRule->match((*f)->getFunction())) {
                continue;
            }

            BPatch_function* bpFunc = (*f)->getFunction()->getBPFunction();
            BPatch_flowGraph* cfg = bpFunc->getCFG();

            stack<BPatch_loopTreeNode*> nodes;

            // get all outer loops
            BPatch_loopTreeNode* node = cfg->getLoopTree();
            for (vector<BPatch_loopTreeNode*>::iterator child = node->children.begin();
                    child != node->children.end();
                    child++) {
                nodes.push((*child));
            }

            while (!nodes.empty()) {
                node = nodes.top();
                nodes.pop();

                if (checkLoopByName(string(node->name()))) {
                    (*f)->addLoopInstrumentation(node, getLoopCodeName());
                }
                // push children if instrumenting all nesting levels
                if (!loopsOnlyOuter) {
                    for (vector<BPatch_loopTreeNode*>::iterator child = node->children.begin();
                            child != node->children.end();
                            child++) {
                        nodes.push((*child));
                    }
                }
            }
        }
    }

    std::cout << "done: " << current->size() << "/" << baseSet->size() << " functions returned\n\n";

    return current;
}

InstrumentSettings::InstrumentSettings(std::string how) {
    callsites = "";
    functions = "";
    loops = "";

    if (how == "") {
        return;
    }

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    boost::char_separator<char> sep(",");

    //split string at ","
    tokenizer tokens(how, sep);

    for (tokenizer::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter) {
        string key, value;

        if ((*tok_iter).find("=") != string::npos) {
            key = (*tok_iter).substr(0, (*tok_iter).find("="));
            value = (*tok_iter).substr((*tok_iter).find("=") + 1);
        }
        else {
            // default key=value
            key = (*tok_iter);
            value = (*tok_iter);
        }

        if (key == "functions") {
            functions = value;
        }
        else if (key == "loops") {
            loops = value;
        }
        else if (key == "callsites") {
            callsites = value;
        }
    }
}

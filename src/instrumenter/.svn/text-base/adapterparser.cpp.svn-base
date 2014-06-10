/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   adapterparser.cpp
 * @author Jan Mussler
 * @brief Implementation of Adapterparser
 */

#include "adapterparser.h"
#include "xmlparser.h"
#include "snippetfactories.h"
#include "adapterconfig.h"
#include "filterparser.h"
#include "ruletree.h"

using namespace std;
using namespace gi::xml;
using namespace gi::instrumenter;
using namespace gi::generator;
using namespace xercesc;

AdapterParser::AdapterParser() {
}

AdapterParser::~AdapterParser() {
}

/**
 * Handles a list of dependencies, stores them in the configuration
 *
 * @param config adapter config where to store information
 * @param nodeList list of <library> tag elements from the spec file
 */
void handleDependencies(AdapterConfig* config, DOMNodeList* nodeList) {
    for (XMLSize_t i = 0; i < nodeList->getLength(); i++) {
        // library to config
        DOMElement* element = dynamic_cast<DOMElement*> (nodeList->item(i));
        string name = getAttributeValue(element, "name");
        string path = getAttributeValue(element, "path");
        config->addDependency(Dependency(name, path));
    }
}

/**
 *
 * Read all <var> children of element and return a set of variables
 *
 * Stores name,size and type information specified in these attributes
 *
 * @param element the parent element containing the list of variables
 * @return a set of Variable objects, specifying variables to create
 */
setVariable handleVariableList(DOMElement* element) {
    DOMNodeList* list = element->getElementsByTagName(XMLString::transcode("var"));
    setVariable varList;
    for (unsigned int i = 0; i < list->getLength(); i++) {
        DOMElement* element = dynamic_cast<DOMElement*> (list->item(i));
        string name = getAttributeValue(element, "name");
        string type = getAttributeValue(element, "type");
        unsigned int size = getUIntAttributeValue(element, "size");

        varList.insert(gi::instrumenter::Variable(name, type, size));
    }
    return varList;
}

/**
 *
 * handles the <adapterfilter> tag in the adapter specification
 * creates a top level OR rule to contain the rules found in the spec
 *
 * @param config
 * @param doc
 */
void handleAdapterFilter(AdapterConfig* config, DOMDocument* doc) {
    DOMNodeList* list;

    list = doc->getElementsByTagName(XMLString::transcode("adapterfilter"));
    if (list->getLength() == 1) {
        gi::xml::FilterParser fp;
        gi::filter::OR* ruleOr = new gi::filter::OR();

        //add all the rules on the top level of adapter filter to one or
        fp.parseNodeContainingRules(ruleOr, list->item(0));
        config->setAdapterFilterRule(ruleOr);
    }
}

bool handleCodeNode(AdapterConfig* config, DOMNode* node) {
    string tag = getTagName(node);
    string name;

    DOMElement* element = dynamic_cast<DOMElement*> (node);

    // <code name="functions" is equal to <functions
    if (tag == "functions" || tag == "loops" || tag == "callsites") {
        name = tag;
    }
    else if (tag == "code") {
        name = getAttributeValue(element, "name");
    }
    else {
        assert(false);
    }

    string initCodeText = getTextFromNode(element, "init");
    string beforeCodeText = getTextFromNode(element, "before");
    string enterCodeText = getTextFromNode(element, "enter");
    string exitCodeText = getTextFromNode(element, "exit");
    string afterCodeText = getTextFromNode(element, "after");
    string finalizeCodeText = getTextFromNode(element, "finalize");

    gi::generator::parser::CodeParser cp;

    bool syntaxError = false;

    if (initCodeText != "") {
        IExpression* init = cp.getIExprAST(initCodeText);

        if (init == 0) {
            std::cout << "[Error]: Syntax Error in <init> code of \"" << name << "\"\n";
            syntaxError = true;
        }

        DOMElement* initElem = getFirstChildElementByName(element, "init");

        string strPrio = getAttributeValue(initElem, "priority");

        int prio = 0;
        stringstream ss(strPrio);
        ss >> prio;

        if (strPrio == "" || ss.fail()) {
            config->addInitExpression(init, name);
        }
        else {
            if(prio < 0) {
                prio = 0; // min prio 0, below is reserved for cobi
                std::cout << "[Warning]: Prio below 0 not allowed in \"" << name << "\"\n";
            }
            config->addInitExpression(init, name, prio);
        }
    }

    if (beforeCodeText != "") {
        IExpression* before = cp.getIExprAST(beforeCodeText);

        if (before == 0) {
            std::cout << "[Error]: Syntax Error in <before> code of \"" << name << "\"\n";
            syntaxError = true;
        }

        config->addBeforeExpression(before, name);
    }

    if (enterCodeText != "") {
        IExpression* enter = cp.getIExprAST(enterCodeText);
        if (enter == 0) {
            std::cout << "[Error]: Syntax Error in <enter> code of \"" << name << "\"\n";
            syntaxError = true;
        }
        config->addEnterExpression(enter, name);
    }

    if (exitCodeText != "") {
        IExpression* exit = cp.getIExprAST(exitCodeText);
        if (exit == 0) {
            std::cout << "[Error]: Syntax Error in <exit> code of \"" << name << "\"\n";
            syntaxError = true;
        }
        config->addExitExpression(exit, name);
    }

    if (afterCodeText != "") {
        IExpression* after = cp.getIExprAST(afterCodeText);
        if (after == 0) {
            std::cout << "[Error]: Syntax Error in <after> code of \"" << name << "\"\n";
            syntaxError = true;
        }
        config->addAfterExpression(after, name);
    }

    if (finalizeCodeText != "") {
        IExpression* finalize = cp.getIExprAST(finalizeCodeText);
        if (finalize == 0) {
            std::cout << "[Error]: Syntax Error in <finalize> code of \"" << name << "\"\n";
            syntaxError = true;
        }
        config->addFinalizeExpression(finalize, name);
    }

    // handle context local variables
    setVariable varList = handleVariableList(element);
    if (varList.size() > 0) {
        config->addVariableList(varList, name);
    }

    return syntaxError;
}

bool handleCode(AdapterConfig* config, DOMDocument* doc) {
    DOMNodeList* list;

    bool syntaxError = false;

    list = doc->getElementsByTagName(XMLString::transcode("functions"));
    if (list->getLength() == 1) {
        syntaxError |= handleCodeNode(config, list->item(0));
    }
    else if (list->getLength() > 1) {
        std::cout << "[Warning]: <functions> element only allowed once in adapter!\n";
    }

    list = doc->getElementsByTagName(XMLString::transcode("loops"));
    if (list->getLength() == 1) {
        syntaxError |= handleCodeNode(config, list->item(0));
    }
    else if (list->getLength() > 1) {
        std::cout << "[Warning]: <loops> element only allowed once in adapter!\n";
    }

    list = doc->getElementsByTagName(XMLString::transcode("callsites"));
    if (list->getLength() == 1) {
        syntaxError |= handleCodeNode(config, list->item(0));
    }
    else if (list->getLength() > 1) {
        std::cout << "[Warning]: <callsites> element only allowed once in adapter!\n";
    }

    list = doc->getElementsByTagName(XMLString::transcode("code"));
    for (XMLSize_t i = 0; i < list->getLength(); ++i) {
        syntaxError |= handleCodeNode(config, list->item(i));
    }

    /* handle adapter init and finalize code */
    {
        gi::generator::parser::CodeParser cp;

        string adapterInit = gi::xml::getTextFromNode(doc->getDocumentElement(),"adapterinitialization");
        string adapterFini = gi::xml::getTextFromNode(doc->getDocumentElement(),"adapterfinalization");

        if(adapterInit != "") {
            IExpression* exp = cp.getIExprAST(adapterInit);
            if(exp==0) {
                std::cout << "[Warning]: Adapterinitialization code is erroneous!\n";
                syntaxError = true;
            }
            config->setAdapterInitCode(exp);
        }

        if(adapterFini != "") {
            IExpression* exp = cp.getIExprAST(adapterFini);
            if(exp==0) {
                std::cout << "[Warning]: Adapterfinalization code is erroneous!\n";
                syntaxError= true;
            }
            config->setAdapterInitCode(exp);
        }
    }

    return syntaxError;
}

/**
 * This function locates the <globalvars> tag and reads the variables listed below,
 * and saves them in the adapter configuration
 *
 * @param config AdapterConfig that is created from doc
 * @param doc XML Document containing the adapter spec
 */
void handleGlobalVariables(AdapterConfig* config, DOMDocument* doc) {
    DOMNodeList* list;
    list = doc->getElementsByTagName(XMLString::transcode("globalvars"));
    if (list->getLength() == 1) {
        DOMElement* element = dynamic_cast<DOMElement*> (list->item(0));
        config->setGlobalVariableList(handleVariableList(element));
    }
}

/**
 * handle adapter specific settings described in adapter specification file
 *
 * this includes
 *
 * 1) saving of all floating point registers required?
 * 2) reading the list of non returning functions that are known or added, and transfered to dyninst
 *
 * @param config
 * @param doc
 */
void handleAdapterSettings(AdapterConfig* config, DOMDocument* doc) {
    DOMNodeList* list;
    list = doc->getElementsByTagName(XMLString::transcode("adaptersettings"));
    if (list->getLength() == 1) {
        DOMElement* element = dynamic_cast<DOMElement*> (list->item(0));

        // <saveallfprs value="true" />

        DOMNodeList* saveList = element->getElementsByTagName(XMLString::transcode("saveallfprs"));
        if (saveList->getLength() == 1) {
            string val = getAttributeValue(dynamic_cast<DOMElement*> (list->item(0)), "value");
            if (val == "true") {
                config->setSaveAllFPRs(true);
            }
        }

        /*
         <nonreturningfunctions>a b c</nonreturningfunctions>
         *
         * function names will not contain " " as they are C functions in base libraries
         * and " " is not allowed within C names
         */
        DOMNodeList* retFuncList = element->getElementsByTagName(XMLString::transcode("nonreturningfunctions"));
        if (retFuncList->getLength() == 1) {
            set<string> funcList = gi::xml::getSetOfTokens(dynamic_cast<DOMElement*> (retFuncList->item(0)));
            for (set<string>::const_iterator i = funcList.begin(); i != funcList.end(); i++) {
                config->addNonReturningFunction(*i);
            }
        }


        /* <initfunctionname value="main" />
         * 
         * Name the function where init code will be placed
         *
         * main or __libc_csu_init (latter one will be before any static constructors are called!
         * */
        DOMNodeList* initFuncList = element->getElementsByTagName(XMLString::transcode("initfunctionname"));
        if (initFuncList->getLength() == 1) {
            string val = getAttributeValue(dynamic_cast<DOMElement*> (initFuncList->item(0)), "value");
            config->setFunctionForInitCode(val);
        }

        /* <initfunctionname value="main" />
         *
         * Name the function where init code will be placed
         *
         * main, _start, or __libc_csu_init (latter one will be before any static constructors are called!
         * */
        DOMNodeList* finalizeFuncList = element->getElementsByTagName(XMLString::transcode("finalizefunctionname"));
        if (finalizeFuncList->getLength() == 1) {
            string val = getAttributeValue(dynamic_cast<DOMElement*> (finalizeFuncList->item(0)), "value");
            config->setFunctionForFinalize(val);
        }
    }
}

IReadableAdapterConfig* AdapterParser::getConfigFromFile(string fileName, bool abortOnError) {

    Parser* xmlparser = Parser::getParser();

    DOMDocument* doc = xmlparser->parseFile(fileName);
    if(doc==0) {
        return 0;
    }

    AdapterConfig* config = new AdapterConfig();

    handleDependencies(config, doc->getElementsByTagName(XMLString::transcode("library")));

    handleCode(config, doc);

    handleAdapterFilter(config, doc);

    handleGlobalVariables(config, doc);

    handleAdapterSettings(config, doc);

    doc->release();

    return config;
}

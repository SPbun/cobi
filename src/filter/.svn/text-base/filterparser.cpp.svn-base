/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filterparser.cpp
 * @author Jan Mussler
 * @brief Implementation of the filter parser
 */

#include "filterparser.h"
#include <string>
#include "xmlparser.h"

using namespace xercesc;
using namespace gi::xml;
using namespace gi::filter;

using namespace std;

/**
 * Constructor
 *   register the given node handlers
 *
 */
FilterParser::FilterParser() {
    registerHandler("AND", new xml::nodehandlers::NH_AND(this));
    registerHandler("OR", new xml::nodehandlers::NH_OR(this));
    registerHandler("NOT", new xml::nodehandlers::NH_NOT(this));
    registerHandler("NAMES", new xml::nodehandlers::NH_Names(this));
    registerHandler("FUNCTIONNAMES", new xml::nodehandlers::NH_Functionnames(this));
    registerHandler("CLASSNAMES", new xml::nodehandlers::NH_Classnames(this));
    registerHandler("MODULENAMES", new xml::nodehandlers::NH_Modulenames(this));
    registerHandler("NAMESPACES", new xml::nodehandlers::NH_Namespaces(this));
    registerHandler("PROPERTY", new xml::nodehandlers::NH_Property(this));
    registerHandler("TRUE", new xml::nodehandlers::NH_True(this));
    registerHandler("FALSE", new xml::nodehandlers::NH_False(this));
}

bool FilterParser::isRegisteredRuleNode(string tagName) {
    transform(tagName.begin(), tagName.end(), tagName.begin(), (int(*)(int))toupper);

    return nodeHandlers.find(tagName) != nodeHandlers.end();
}

bool FilterParser::isFilterElementNode(string tagName) {
    return tagName == "include"
            || tagName == "exclude";
}

/**
 * Destructor
 *   delete the registered node handlers associated with this parser
 */
FilterParser::~FilterParser() {
    for (map<string, INodeHandler*>::iterator i = nodeHandlers.begin();
            i != nodeHandlers.end(); i++) {
        delete (*i).second;
    }
    nodeHandlers.clear();
}

/**
 *  Parse the given DOMElement
 *      if the tag name is registered with a handler call the handler
 *      which then creates the proper rule and continues parsing down the DOM tree
 *
 *      handlers store a reference to the parser for the recursive call
 *
 * @param DOMElement to be evaluated
 * @return tree of rules
 */
IRule* FilterParser::parseRuleNode(DOMElement* element) {
    string tagName = getTagName(element);
    // upper case used in map
    transform(tagName.begin(), tagName.end(), tagName.begin(), (int(*)(int))toupper);

    if (nodeHandlers.find(tagName) != nodeHandlers.end()) {
        return nodeHandlers[tagName]->handleNode(element);
    }
    else {
        throw ex::UnknownRuleNodeException();
    }
}

/**
 * parse include element and its children
 */
void FilterParser::handleIncludeNode(DOMNode* node, Filter* f) {
    AND* andNode = new AND();

    parseNodeContainingRules(andNode, node);

    if (andNode->count() > 0) {
        f->addElement(new IncludeElement(andNode));
    }
}

/**
 * parse exclude element and its children
 */
void FilterParser::handleExcludeNode(DOMNode* node, Filter* f) {
    OR* orNode = new OR();

    parseNodeContainingRules(orNode, node);

    if (orNode->count() > 0) {
        f->addElement(new ExcludeElement(orNode));
    }
}

void FilterParser::parseNodeContainingRules(ARuleSet* parentSet, DOMNode* node) {
    for (DOMNode* child = node->getFirstChild();
            child != 0;
            child = child->getNextSibling()) {
        try {
            if (isRegisteredRuleNode(getTagName(child))) {
                DOMElement* childElement = dynamic_cast<DOMElement*> (child);
                IRule* newRule = parseRuleNode(childElement);
                assert(newRule);
                parentSet->addRule(newRule);
            }
        }        catch (ex::FilterParserException) {
            std::cout << "exception: error while parsing exclude filterelement\n";
        }
    }
}

IFilter* FilterParser::parseFilterNode(DOMElement* element) {
    assert(fs);

    string name = getAttributeValue(element, "name");
    string instrument = getAttributeValue(element, "instrument");
    string start = getAttributeValue(element, "start");
    string desc = getAttributeValue(element, "description");

    // create new filter, pass the parent filter set (fs)
    Filter* filter = new Filter(name, start, instrument, fs);
    filter->setDescription(desc);

    DOMNode* node;

    // loop over filter elements
    for (node = element->getFirstChild(); node != 0; node = node->getNextSibling()) {
        string tagName = getTagName(node);

        //TODO: schema evaluation, skipable whitespace
        if (tagName == "#text") continue;

        if (isFilterElementNode(tagName)) {
            // handle nodes of special type
            if (tagName == "include") {
                handleIncludeNode(node, filter);
            }
            else if (tagName == "exclude") {
                handleExcludeNode(node, filter);
            }
        }
        else if (isRegisteredRuleNode(tagName)) {

            // handle nodes on top level not being of type include or exclude
            //
            // an exclude element will be created which when the filter is applied
            // excludes all functions matching the given filter

            // TODO: check if smart default, probably better with NOT() exclude

            DOMElement* e = dynamic_cast<DOMElement*> (node);
            try {
                ExcludeElement* fe = new ExcludeElement(parseRuleNode(e));
                filter->addElement(fe);
            }            catch (ex::FilterParserException) {
                std::cout << "[Exception]: Error while parsing top level rule node\n";
            }
        }
        else if (tagName == "callsites") {
            ARuleSet* rs = new AND();
            parseNodeContainingRules(rs, node);
            if (rs->count() > 0) {
                filter->setCallSiteRule(rs);
            }
            else {
                delete rs;
            }
        }
        else if (tagName == "loops") {
            ARuleSet* rs = new AND();
            parseNodeContainingRules(rs, node);

            string onlyOuter = getAttributeValue(static_cast<DOMElement*> (node), "onlyOuterLoops");
            if (onlyOuter == "yes" || onlyOuter == "true" || onlyOuter == "1") {
                filter->setLoopsOnlyOuter(true);
            }

            string loopNames = getAttributeValue(static_cast<DOMElement*> (node), "names");
            set<string> allowedNames = gi::xml::getTokensFromString(",", loopNames);
            for (set<string>::const_iterator i = allowedNames.begin(); i != allowedNames.end(); i++) {                
                filter->addLoopName(*i);
            }

            if (rs->count() > 0) {
                filter->setLoopRule(rs);
            }
            else {
                delete rs;
            }
        }
        else {
            std::cout << "unexpected node encountered - skipped\n";
        }
    } // filter nodes loop

    return filter;
}

/**
 * Parse Filter XML specification and return a set of filters
 *
 * @return new filter set
 */
FilterSet* FilterParser::parseFile(string fileName) {
    Parser* p = Parser::getParser();
    DOMDocument* doc = p->parseFile(fileName);
    if(doc==0) {
        return 0;
    }

    DOMElement* root = doc->getDocumentElement();
    assert(root != 0);

    std::cout << "=== start parsing filer specification ===\n";
    fs = new FilterSet();

    // retreive all the filter nodes ( other node types ignored, none valid )
    DOMNodeList* filters = root->getElementsByTagName(XMLString::transcode("filter"));
    std::cout << "reading " << filters->getLength() << " filters from file \"" << fileName << "\":\n";
    std::cout << "\t";
    for (unsigned int i = 0; i < filters->getLength(); ++i) {
        DOMElement* e = dynamic_cast<DOMElement*> (filters->item(i));
        IFilter* f = parseFilterNode(e);

        fs->addFilter(f->getName(), f);
        
        std::cout <<  f->getName() << ((i<filters->getLength()-1)?", ":"");

        if(i%3==2) std::cout << "\n\t";
    }

    std::cout << "\n=== end of parsing filter specification ===\n\n";

    doc->release();

    return fs;
}

void FilterParser::registerHandler(string nodeType, INodeHandler* handler) {
    nodeHandlers[nodeType] = handler;
}

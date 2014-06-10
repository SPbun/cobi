/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   nodehandleres.cpp
 * @author Jan Mussler
 * @brief Implementation of nodehandlers
 *
 * Implementation of the different handlers for xml elements read in the
 * filter specification file i.e. the rule tree
 */

#include "patterns.h"
#include "properties.h"
#include "nodehandlers.h"
#include "filterparser.h"

using namespace std;
using namespace gi::xml::nodehandlers;
using namespace gi::xml;
using namespace gi::filter;

INodeHandler::INodeHandler(FilterParser* aFp) : fp(aFp) {
}

NH_AND::NH_AND(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_AND::handleNode(DOMElement* element) {
    AND* andRule = new AND();
    DOMNode* childNode = 0;
    bool hasChildRule = false;

    try {
        for (childNode = element->getFirstChild(); childNode != 0; childNode = childNode->getNextSibling()) {
            std::string tagName = getTagName(childNode);
            if (tagName == "#text") continue;

            if (fp->isRegisteredRuleNode(tagName)) {
                hasChildRule = true;
                DOMElement* childElement = dynamic_cast<DOMElement*> (childNode);
                andRule->addRule(fp->parseRuleNode(childElement));
            }
        }
    }    catch (ex::FilterParserException& e) {
        delete andRule;
        throw e;
    }

    if (!hasChildRule) {
        delete andRule;
        throw ex::NoChildNodesFoundException();
    }

    return andRule;
}

NH_OR::NH_OR(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_OR::handleNode(DOMElement* element) {
    OR* orRule = new OR();
    DOMNode* childNode = 0;
    bool hasChildRule = false;
    try {
        for (childNode = element->getFirstChild(); childNode != 0; childNode = childNode->getNextSibling()) {
            std::string tagName = getTagName(childNode);
            if (tagName == "#text") continue;

            if (fp->isRegisteredRuleNode(tagName)) {
                hasChildRule = true;
                DOMElement* childElement = dynamic_cast<DOMElement*> (childNode);
                orRule->addRule(fp->parseRuleNode(childElement));
            }
        }
    }    catch (ex::FilterParserException& e) {
        delete orRule;
        throw e;
    }

    if (!hasChildRule) {
        delete orRule;
        throw ex::UnknownNodeException();
    }

    return orRule;
}

NH_NOT::NH_NOT(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_NOT::handleNode(DOMElement* element) {
    NOT* notRule = new NOT();
    bool hasChildRule = false;
    bool tooManyChildren = false;
    DOMNode* childNode = 0;

    try {
        for (childNode = element->getFirstChild(); childNode != 0; childNode = childNode->getNextSibling()) {
            std::string tagName = getTagName(childNode);
            if (tagName == "#text") continue;

            if (fp->isRegisteredRuleNode(tagName)) {
                if (!hasChildRule) {
                    hasChildRule = true;
                    DOMElement* childElement = dynamic_cast<DOMElement*> (childNode);
                    notRule->setRule(fp->parseRuleNode(childElement));
                }
                else {
                    tooManyChildren = true;
                    break;
                }
            }
        }
    }    catch (ex::FilterParserException& e) {
        delete notRule;
        throw e;
    }

    if (tooManyChildren) {
        delete notRule;
        throw ex::TooManyNodesFoundException();
    }

    if (!hasChildRule) {
        delete notRule;
        throw ex::NoRuleNodesFoundException();
    }

    return notRule;
}

NH_Functionnames::NH_Functionnames(FilterParser* aFp) : ANH_Patterns(aFp) {
}

IPatterns* NH_Functionnames::getPatternsImpl(IMatchString* strat) {
    return new FunctionNames(strat);
}

NH_Names::NH_Names(FilterParser* aFp) : ANH_Patterns(aFp) {
}

IPatterns* NH_Names::getPatternsImpl(IMatchString* strat) {
    return new Names(strat);
}

ANH_Patterns::ANH_Patterns(FilterParser* aFp) : INodeHandler(aFp) {

}

IRule* ANH_Patterns::handleNode(DOMElement* element) {
    // default match using equals
    IMatchString* strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::EQUALS);

    std::string match = getAttributeValue(element, "match");

    if (match == "regex") {
        strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::REGEX);
    }
    else if (match == "simple") {
        strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::SIMPLE);
    }
    else if (match == "suffix") {
        strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::SUFFIX);
    }
    else if (match == "prefix") {
        strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::PREFIX);
    }
    else if (match == "find") {
        strat = MatchStrategyFactory::getStrategy(MatchStrategyFactory::FIND);
    }

    IPatterns* ip = getPatternsImpl(strat);

    set<string> patternSet = gi::xml::getSetOfTokens(element);

    for (set<string>::const_iterator i = patternSet.begin(); i != patternSet.end(); i++) {
        if (strat->validPattern(*i)) {
            ip->addPattern(*i);
        }
        else {
            throw filter::ex::FilterInvalidPattern();
        }
    }
    
    return ip;
}

NH_Classnames::NH_Classnames(FilterParser* aFp) : ANH_Patterns(aFp) {
}

IPatterns* NH_Classnames::getPatternsImpl(IMatchString* strat) {
    return new ClassNames(strat);
}

NH_Namespaces::NH_Namespaces(FilterParser* aFp) : ANH_Patterns(aFp) {
}

IPatterns* NH_Namespaces::getPatternsImpl(IMatchString* strat) {
    return new Namespaces(strat);
}

NH_Modulenames::NH_Modulenames(FilterParser* aFp) : ANH_Patterns(aFp) {
}

IPatterns* NH_Modulenames::getPatternsImpl(IMatchString* strat) {
    return new ModuleNames(strat);
}

NH_Property::NH_Property(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_Property::handleNode(DOMElement* element) {
    std::string name = getAttributeValue(element, "name");
    IProperty* p = PropertyFactory::Get().getProperty(name);
    p->setUp(fp, element);
    return p;
}

NH_True::NH_True(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_True::handleNode(DOMElement* element) {
    return new RTrue();
}

NH_False::NH_False(FilterParser* aFp) : INodeHandler(aFp) {
}

IRule* NH_False::handleNode(DOMElement* element) {
    return new RFalse();
}

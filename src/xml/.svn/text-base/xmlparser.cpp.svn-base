/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   xmlparser.cpp
 * @author Jan Mussler
 * @brief XML parser and utility functions
 */

#include "xmlparser.h"
#include "browser.h"

#include "boost/tokenizer.hpp"

using namespace std;
using namespace gi;
using namespace gi::xml;
using namespace xercesc;

std::string gi::xml::getTagName(DOMNode* node) {
    return XMLString::transcode(node->getNodeName());
}

std::string gi::xml::getAttributeValue(DOMElement* element, std::string attributeName) {
    return XMLString::transcode(element->getAttribute(XMLString::transcode(attributeName.c_str())));
}

unsigned int gi::xml::getUIntAttributeValue(DOMElement* element, std::string attributeName) {
    string tmp = getAttributeValue(element, attributeName);
    stringstream ss(tmp);
    unsigned int i;
    ss >> i;
    return i;
}

int gi::xml::getIntAttributeValue(DOMElement* element, std::string attributeName) {
    string tmp = getAttributeValue(element, attributeName);
    stringstream ss(tmp);
    int i;
    ss >> i;
    return i;
}

size_t gi::xml::getNumericValue(DOMElement* element, std::string attributeName) {
    string tmp = getAttributeValue(element, attributeName);
    stringstream ss(tmp);
    size_t i;
    ss >> i;
    return i;
}

DOMElement* gi::xml::getFirstChildElementByName(DOMElement* element, string nodeName) {
    DOMNodeList* list = element->getElementsByTagName(XMLString::transcode(nodeName.c_str()));
    if (list->getLength() > 0) {
        return static_cast<DOMElement*> (list->item(0));
    }
    else {
        return 0;
    }
}

/**
 * Locates the first child of element with given name and
 * returns its text value if of type #text or #cdata-section
 *
 * returns empty string of none or more than 1 element is found with name
 *
 * @param element parent element
 * @param nodeName name of tag to look for below element
 * @return text data of element with tag name
 */
string gi::xml::getTextFromNode(DOMElement* element, string nodeName) {
    DOMNodeList* list = element->getElementsByTagName(XMLString::transcode(nodeName.c_str()));
    if (list->getLength() == 1) {
        DOMNode* node = list->item(0)->getFirstChild();
        if (node != 0) {
            if (getTagName(node) == "#text" || getTagName(node) == "#cdata-section") {
                return XMLString::transcode(node->getNodeValue());
            }
        }
    }
    return "";
}

set<string> gi::xml::getTokensFromString(string separator, string list) {
    set<string> resultSet;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    boost::char_separator<char> sep(separator.c_str());

    tokenizer tokens(list, sep);

    for (tokenizer::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter) {
        string tmpPat = *tok_iter;

        while (tmpPat.find("\n") != tmpPat.npos)
            tmpPat.replace(tmpPat.find("\n"), 1, "");

        while (tmpPat.find(" ") != tmpPat.npos)
            tmpPat.replace(tmpPat.find(" "), 1, "");

        while (tmpPat.find("\t") != tmpPat.npos)
            tmpPat.replace(tmpPat.find("\t"), 1, "");

        if (tmpPat.length() > 0) {
            resultSet.insert(tmpPat);
        }
    }

    return resultSet;

}

set<string> gi::xml::getSetOfTokens(DOMElement* element) {
    DOMNode* node;
    set<string> resultSet;

    for (node = element->getFirstChild(); node != 0; node = node->getNextSibling()) {
        if (getTagName(node) == "#text" || getTagName(node) == "#cdata-section") {

            string content = XMLString::transcode(node->getNodeValue());

            typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

            boost::char_separator<char> sep("\n ");

            tokenizer tokens(content, sep);

            for (tokenizer::iterator tok_iter = tokens.begin();
                    tok_iter != tokens.end(); ++tok_iter) {
                string tmpPat = *tok_iter;

                while (tmpPat.find("\n") != tmpPat.npos)
                    tmpPat.replace(tmpPat.find("\n"), 1, "");

                while (tmpPat.find(" ") != tmpPat.npos)
                    tmpPat.replace(tmpPat.find(" "), 1, "");

                while (tmpPat.find("\t") != tmpPat.npos)
                    tmpPat.replace(tmpPat.find("\t"), 1, "");

                if (tmpPat.length() > 0) {
                    resultSet.insert(tmpPat);
                }
            }
        }
    }

    return resultSet;
}

Parser* Parser::p = 0;

Parser::Parser() {
    parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Never);
    parser->setDoNamespaces(false);
    parser-> setIncludeIgnorableWhitespace(false);
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
}

Parser::Parser(Parser& p) {

}

Parser::~Parser() {

}

DOMDocument* Parser::parseFile(std::string fileName) {
    try {
        assert(parser != 0);
        parser->parse(fileName.c_str());
        return parser->getDocument();
    } catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "[Error]: " << message << "in line: " << toCatch.getSrcLine() << "\n";
        XMLString::release(&message);
        return 0;
    } catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        cout << "DOMException message is: \n"
                << message << "\n";
        XMLString::release(&message);
        return 0;
    } catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "[Error]: " << message << " in line: " << toCatch.getLineNumber() << "\n";
        XMLString::release(&message);
        return 0;
    } catch (...) {
        cout << "Unexpected Exception \n";
        return 0;
    }
}

Parser* Parser::getParser() {
    if (p == 0) {
        try {
            XMLPlatformUtils::Initialize();
        } catch (const XMLException& toCatch) {
            return 0;
        }
        p = new Parser();
    }
    return p;
}

void Parser::Destroy() {
    delete p;
    p = 0;

    XMLPlatformUtils::Terminate();
}

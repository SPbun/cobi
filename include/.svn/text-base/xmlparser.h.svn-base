/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   xmlparser.h
 * @author Jan Mussler
 * @brief XML Parser definitions
 */

#ifndef _XMLPARSER_H
#define	_XMLPARSER_H

#include <string>
#include <map>
#include <set>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc;

namespace gi {
    namespace xml {

        /**
         * get tag name of node
         * @param node xml element
         * @return tag name of element
         */
        std::string getTagName(DOMNode* node);

        /**
         * @param element xml element
         * @param attributeName name of attribute
         * @return attribute value
         */
        std::string getAttributeValue(DOMElement* element, std::string attributeName);

        DOMElement* getFirstChildElementByName(DOMElement* element, std::string nodeName);

        /**
         * @param element xml element
         * @param attributeName name of attribute
         * @return attribute value
         */
        unsigned int getUIntAttributeValue(DOMElement* element, std::string attributeName);

        int getIntAttributeValue(DOMElement* element, std::string attributeName);

        size_t getNumericValue(DOMElement* element, std::string attributeName);

        std::string getTextFromNode(DOMElement* element, std::string nodeName);

        std::set<std::string> getTokensFromString(std::string separator, std::string list);

        std::set<std::string> getSetOfTokens(DOMElement* element);

        /**
         * XML Parser
         *   using apache xerces-c++ api
         */
        class Parser {
        private:
            static Parser* p;
            XercesDOMParser* parser;
            Parser();
            ~Parser();
            Parser(Parser& p);
        public:
            static Parser* getParser();
            static void Destroy();
            DOMDocument* parseFile(std::string fileName);
        };
    }
}

#endif	/* _XMLPARSER_H */

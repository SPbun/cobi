/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filterparser.h
 * @author Jan Mussler
 * @brief Filterparser definitions
 */

#ifndef _FILTERPARSER_H
#define	_FILTERPARSER_H

#include "xmlparser.h"

#include "filters.h"
#include "patterns.h"

#include "nodehandlers.h"

using namespace xercesc;

namespace gif = gi::filter;

namespace gi {
    namespace xml {
        /**
         * Parser for a filter specification files
         */
        class FilterParser {
        private:
            std::map<std::string, INodeHandler*> nodeHandlers;
            gif::FilterSet* fs;

            /**
             * parse a exclode domnode
             */
            void handleExcludeNode(DOMNode* node, gif::Filter* f);

            /**
             * parse a include domnode
             */
            void handleIncludeNode(DOMNode* node, gif::Filter* f);
        public:
            FilterParser();
            ~FilterParser();

            //TODO consider static parser
            /**
             * parse a node, which itself contains cildnodes and appends the created rules to the topNode
             * @param topNode node to which to add the generated rules
             * @param node the node who's children will be parse
             */
            void parseNodeContainingRules(gif::ARuleSet* topNode, DOMNode* node);

            /**
             * check whether there tagname is registered as a parsable rule node
             * @return true if tagname matches registered rule node
             */
            bool isRegisteredRuleNode(std::string tagName);

            /**
             * check whether element node is special filter element
             * @return tagName equals include, exclude
             */
            bool isFilterElementNode(std::string tagName);

            /**
             * @param element in spec file describing filter
             * @return filter
             */
            gif::IFilter* parseFilterNode(DOMElement* element);

            /**
             * parse element node and create rule from there
             * @return rule to be associated with filter
             */
            gif::IRule* parseRuleNode(DOMElement* element);

            /**
             * @param fileName name of filter specification file
             * @return set of filters specified in file
             */
            gif::FilterSet* parseFile(std::string fileName);

            /**
             * register handlers for filter nodes
             * @param nodeType tag name of xml element
             * @param handler object which handles node of that type
             */
            void registerHandler(std::string nodeType, INodeHandler* handler);
        };
    }
}

#endif	/* _FILTERPARSER_H */

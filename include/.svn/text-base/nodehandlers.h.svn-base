/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   nodehandler.h
 * @author Jan Mussler
 * @brief Nodehandler class definitions
 */

#ifndef _NODEHANDLERS_H
#define	_NODEHANDLERS_H

#include "xmlparser.h"

#include "filters.h"
#include "patterns.h"

using namespace xercesc;
using namespace gi::filter;

namespace gi {
    namespace xml {

        class FilterParser;

        /**
         * interface for handling filter spec xml elements like and, or, not, functionnames,...
         **/
        class INodeHandler {
        protected:
            /**
             * store filerparser, needed to parse element
             */
            FilterParser* fp;
        public:
            virtual ~INodeHandler() {};
            
            INodeHandler(FilterParser* aFp);

            /**
             * parse the given domelement and return a new rule
             */
            virtual IRule* handleNode(DOMElement* element) = 0;
        };

        namespace nodehandlers {

            /**
             * handler for "and" nodes
             */
            class NH_AND : public INodeHandler {
            public:
                NH_AND(FilterParser* aFp);
                /**
                 * create an filter::and node
                 */
                IRule* handleNode(DOMElement* element);
            };

            /**
             * handler for "or" nodes
             */
            class NH_OR : public INodeHandler {
            public:
                NH_OR(FilterParser* aFp);
                /**
                 * create filter::or node
                 */
                IRule* handleNode(DOMElement* element);
            };

            /**
             * handler for "not" nodes
             */
            class NH_NOT : public INodeHandler {
            public:
                NH_NOT(FilterParser* aFp);

                /**
                 * create filter::not node
                 */
                IRule* handleNode(DOMElement* element);
            };

            /**
             * Base class for all node handling of patterns nodes
             *   provides #text element parser with handleNode() and template method
             *   for creating the correct ipatterns implementation object
             */
            class ANH_Patterns : public INodeHandler {
            protected:
                /**
                 * template method, to be overridden by subclasses for creating
                 * the required object
                 * @param strat matching strategy passed on to created object
                 * @return object implementing IPatterns
                 */
                virtual IPatterns* getPatternsImpl(IMatchString* strat) = 0;
            public:
                virtual ~ANH_Patterns() {};
                /**
                 * @param aFp FilterParser for recursive call
                 */
                ANH_Patterns(FilterParser* aFp);

                /**
                 * parse element node and create ipatterns object
                 *   add patterns specified in child #text node to ipatterns object
                 *
                 * @param element XML element
                 * @return rule tree
                 */
                virtual IRule* handleNode(DOMElement* element);
            };

            /**
             * handler for "functionnames" nodes
             */
            class NH_Functionnames : public ANH_Patterns {
            protected:
                IPatterns* getPatternsImpl(IMatchString* strat);
            public:

                NH_Functionnames(FilterParser* aFp);
            };

            /**
             * handler for "functionnames" nodes
             */
            class NH_Names : public ANH_Patterns {
            protected:
                IPatterns* getPatternsImpl(IMatchString* strat);
            public:
                NH_Names(FilterParser* aFp);
            };

            /**
             * handler for "functionnames" nodes
             */
            class NH_Classnames : public ANH_Patterns {
            protected:
                IPatterns* getPatternsImpl(IMatchString* strat);
            public:
                NH_Classnames(FilterParser* aFp);
            };

            /**
             * handler for "functionnames" nodes
             */
            class NH_Namespaces : public ANH_Patterns {
            protected:
                IPatterns* getPatternsImpl(IMatchString* strat);
            public:
                NH_Namespaces(FilterParser* aFp);
            };

            /**
             * handler for "modulnames" nodes
             */
            class NH_Modulenames : public ANH_Patterns {
            protected:
                IPatterns* getPatternsImpl(IMatchString* strat);
            public:
                NH_Modulenames(FilterParser* aFp);
            };

            /**
             * handler for "property" nodes
             */
            class NH_Property : public INodeHandler {
            public:
                NH_Property(FilterParser* aFp);
                IRule* handleNode(DOMElement* element);
            };

            /**
             * handler for "true" nodes
             */
            class NH_True : public INodeHandler {
            public:
                NH_True(FilterParser* aFp);
                IRule* handleNode(DOMElement* element);
            };

            /**
             * handler for "false" nodes
             */
            class NH_False : public INodeHandler {
            public:
                NH_False(FilterParser* aFp);
                IRule* handleNode(DOMElement* element);
            };
        }
    }
}

#endif	/* _NODEHANDLERS_H */

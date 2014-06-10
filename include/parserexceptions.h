/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   parserexceptions.h
 * @author Jan Mussler
 * @brief Exceptions thrown during parsing of filter
 */

#ifndef _PARSEREXCEPTIONS_H
#define	_PARSEREXCEPTIONS_H

namespace gi {
    namespace filter {
        namespace ex {

            /**
             * Exceptions in filter parser
             */
            class FilterParserException {
            };

            /**
             * no child elements found where expected
             */
            class NoChildNodesFoundException : public FilterParserException {
            };

            /**
             * no rule elements found where expected e.g. and/or
             */
            class NoRuleNodesFoundException : public NoChildNodesFoundException {
            };

            /**
             * no text node found where expected ( functionnames, modulenames... )
             */
            class NoTextNodeFoundException : public FilterParserException {
            };

            /**
             * Found too many nodes ( not with more children than 1 )
             */
            class TooManyNodesFoundException : public FilterParserException {
            };

            /**
             * unknown element encountered while reading xml file
             */
            class UnknownNodeException : public FilterParserException {
            };

            /**
             * unknown rule element found while parsing
             */
            class UnknownRuleNodeException : public UnknownNodeException {
            };

            class UnknownPropertyException : public FilterParserException {
                
            };

            /**
             * referecing a named filter, which has not been defined
             */
            class ReferencingUndefinedFilter : public FilterParserException {
            };

            /**
             * provided regular expressions are invalid
             */
            class FilterInvalidPattern : public FilterParserException {
                
            };

            /**
             * referencing a startset which has not been computed
             * //TODO ? handle by computing it if defined
             */
            class UsingUnevaluatedFilterAsStartSet : public FilterParserException {
            };
        }
    }
};

#endif	/* _PARSEREXCEPTIONS_H */

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   patterns.h
 * @author Jan Mussler
 * @brief All pattern related class defintions
 */

#ifndef _PATTERNS_H
#define	_PATTERNS_H

#include <map>
#include <vector>

#include "ruletree.h"

namespace gi {
    namespace filter {

        /**
         * Match String Strategy Interface
         */
        class IMatchString {
            friend class MatchStrategyFactory;
        protected:
            IMatchString();
            virtual ~IMatchString() {};
        public:

            /**
             * @return true if test matches pattern
             */
            virtual bool match(std::string pattern, std::string test) const = 0;
            
            /**
             * @return true if provided pattern is valid ( relevant for regular expression patterns )
             */
            virtual bool validPattern(std::string pattern) { return true; };
        };

        /**
         * Base class for pattern filters using functionname, modulname, ...
         */
        class IPattern : public IRule {
        protected:
            /**
             * strategy used in matching to strings
             *   either char by char, simple pattern or regular expression
             */
            const IMatchString * const matchStrategy;
        protected:
            virtual ~IPattern() {};
            /**
             * @param strategy matching strategy
             */
            IPattern(IMatchString* strategy);
        };

        /**
         * base class for all filters using a list of patterns to match with function
         *   stores patterns and provides implementation for matching
         *   input function against stored patterns
         */
        class IPatterns : public IPattern {
        protected:
            std::vector<std::string> patterns;
            /**
             * template method to be overriden
             *   returns the requested name of the function e.g. class or function name
             *
             * @param f input function
             * @return requested name
             */
            virtual std::string getName(gi::mutatee::IFunction* f) = 0;

            /**
             * @param strategy matching strategy
             */
            IPatterns(IMatchString* strategy);
        public:
            virtual ~IPatterns() {};

            /**
             * Add pattern to list of patterns
             * @param aPattern pattern to add
             */
            void addPattern(std::string aPattern);

            /**
             * Try to match name with any of the stored patterns
             *   using template function getName()
             *
             * @param f function to inspect
             * @return wether function matched any of the patterns
             */
            virtual bool match(gi::mutatee::IFunction* f);
        };

        /**
         * Functionnames rule
         *   contains a list of patterns and returns true if function matches any pattern
         */
        class Names : public IPatterns {
        protected:
            virtual std::string getName(gi::mutatee::IFunction* f);
        public:
            virtual ~Names() {};
            /**
             * @param strategy strategy used for matching function names and pattern
             */
            Names(IMatchString* strategy);
        };

        /**
         * Functionnames rule
         *   contains a list of patterns and returns true if function matches any pattern
         */
        class FunctionNames : public IPatterns {
        protected:
            virtual std::string getName(gi::mutatee::IFunction* f);
        public:
            virtual ~FunctionNames() {};
            /**
             * @param strategy strategy used for matching function names and pattern
             */
            FunctionNames(IMatchString* strategy);
        };

        /**
         * Functionnames rule
         *   contains a list of patterns and returns true if function matches any pattern
         */
        class ClassNames : public IPatterns {
        protected:
            virtual std::string getName(gi::mutatee::IFunction* f);
        public:
            virtual ~ClassNames() {};
            /**
             * @param strategy strategy used for matching function names and pattern
             */
            ClassNames(IMatchString* strategy);
        };

        /**
         * Functionnames rule
         *   contains a list of patterns and returns true if function matches any pattern
         */
        class Namespaces : public IPatterns {
        protected:
            virtual std::string getName(gi::mutatee::IFunction* f);
        public:
            virtual ~Namespaces() {};
            /**
             * @param strategy strategy used for matching function names and pattern
             */
            Namespaces(IMatchString* strategy);
        };

        /**
         * Modulenames rule
         *   contains a list of patterns and returns true if function's modulename matches any pattern
         */
        class ModuleNames : public IPatterns {
        protected:
            virtual std::string getName(gi::mutatee::IFunction* f);
        public:
            virtual ~ModuleNames() {};
            /**
             * @param strategy strategy used for matching function names and pattern
             */
            ModuleNames(IMatchString* strategy);
        };

        /**
         * Pattern matching against function name
         */
        class FunctionName : public IPattern {
        private:
            std::string name;
        public:
            virtual ~FunctionName() {};
            FunctionName(IMatchString* strategy, std::string aName);
            virtual bool match(gi::mutatee::IFunction* f);
        };

        /**
         * Pattern matching against modul name
         */
        class ModuleName : public IPattern {
        private:
            std::string name;
        public:
            virtual ~ModuleName() {};
            ModuleName(IMatchString* strategy, std::string aName);
            virtual bool match(gi::mutatee::IFunction* f);
        };

        /**
         * Strategy for matching strings characater for character "=="
         */
        class MatchEquals : public IMatchString {
        protected:
            virtual ~MatchEquals() {};
        public:
            /**
             * @return true if both strings are equal
             */
            virtual bool match(std::string pattern, std::string test) const;
        };

        /**
         * Strategy for matching using * at the end
         */
        class MatchSimplePattern : public IMatchString {
        protected:
            virtual ~MatchSimplePattern() {};
        public:
            /**
             * @return true if test matches the pattern
             */
            virtual bool match(std::string pattern, std::string test) const;
        };

        /**
         * Strategy for matching using regular expressions
         */
        class MatchRegex : public IMatchString {
        protected:
            virtual ~MatchRegex() {};
        public:
            /**
             * @return true if test matches the regular expression pattern
             */
            virtual bool match(std::string pattern, std::string test) const;
            virtual bool validPattern(std::string pattern) const;
        };

        class MatchPrefix : public IMatchString {
        protected:
            virtual ~MatchPrefix() {};
        public:
            virtual bool match(std::string pattern, std::string test) const;
        };

        class MatchSuffix : public IMatchString {
        protected:
            virtual ~MatchSuffix() {};
        public:
            virtual bool match(std::string pattern, std::string test) const;
        };

        class MatchFind : public IMatchString {
        protected:
            virtual ~MatchFind() {};
        public:
            virtual bool match(std::string pattern, std::string test) const;
        };

        /**
         * Factory for all Match Strategies used in patterns
         */
        class MatchStrategyFactory {
        public:

            enum strategies {
                EQUALS, SIMPLE, REGEX, PREFIX, SUFFIX, FIND
            };
            
        private:
            /**
             * static map, will be initialised by first use
             */
            static std::map<strategies, IMatchString*> strategymap;
        public:
            /**
             * @return pointer to specified strategy
             */
            static IMatchString * const getStrategy(strategies s);
        };
    }
}
#endif	/* _PATTERNS_H */

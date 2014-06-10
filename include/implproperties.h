/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   implproperties.h
 * @author Jan Mussler
 * @brief Definitions of all implemented properties
 */

#ifndef _IMPLPROPERTIES_H
#define	_IMPLPROPERTIES_H

#include "properties.h"

// TODO new property class for all min max properties?

using namespace gi::filter;
namespace gix = gi::xml;
namespace gim = gi::mutatee;

namespace gi {
    namespace filter {
        namespace properties {

            /**
             * Call Path Filter
             */
            class CallPath : public IProperty {
            private:
                gim::graph::GraphQueryResult* namesOnPath;
                IRule* targetRule;
                bool printSetAfterInit;
                bool forward;
            public:
                /**
                 * used to setup a property with the domelement from the filter spec
                 */
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);

                /**
                 * used to do one time calculation before filtering the mutatee
                 */
                virtual bool init(gim::Mutatee* mutatee);

                /**
                 * if property needs initialisation, return true
                 */
                virtual bool needsInit();

                virtual bool match(gim::IFunction* f);

                virtual ~CallPath();
            };

            /**
             * Depth in Call Tree from main() Filter
             */
            class Depth : public IProperty {
            private:
                string originName;
                unsigned int depth;
                gim::graph::GraphQueryResult* namesWithinDepth;
            public:
                /**
                 * used to setup a property with the domelement from the filter spec
                 */
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);

                /**
                 * used to do one time calculation before filtering the mutatee
                 */
                virtual bool init(gim::Mutatee* mutatee);

                /**
                 * if property needs initialisation, return true
                 */
                virtual bool needsInit();

                virtual bool match(gim::IFunction* f);

                virtual ~Depth();
            };

            class Mccabe : public IProperty {
            private:
                size_t minValue;
                size_t maxValue;
            public:
                Mccabe();

                /**
                 * used to setup a property with the domelement from the filter spec
                 */
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);

                /**
                 * used to do one time calculation before filtering the mutatee
                 */
                virtual bool init(gim::Mutatee* mutatee);

                /**
                 * if property needs initialisation, return true
                 */
                virtual bool needsInit();

                virtual bool match(gim::IFunction* f);

                virtual ~Mccabe();
            };

            /**
             * Lines of Code Metric Property
             * retrieve lines of code from debug information
             * */
            class SourceVolume : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
                gim::Mutatee* mutatee;
            public:
                SourceVolume();

                /**
                 * used to setup a property with the domelement from the filter spec
                 */
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);

                /**
                 * used to do one time calculation before filtering the mutatee
                 */
                virtual bool init(gim::Mutatee* mutatee);

                /**
                 * if property needs initialisation, return true
                 */
                virtual bool needsInit();

                virtual bool match(gim::IFunction* f);

                virtual ~SourceVolume();
            };

            class IsLibraryFunction : public IProperty {
            public:
                IsLibraryFunction();
                virtual ~IsLibraryFunction();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            /**
             * Property to check whether function has at least minValue number
             * of loops, with nesting of level.
             *
             * Nesting equals depth in loop tree Dyninst provides
             */
            class HasLoop : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
                unsigned int level;
            public:
                HasLoop();
                virtual ~HasLoop();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            class CountCallSites : public IProperty {
            private:
                IRule* rule;
                unsigned int minValue;
                unsigned int maxValue;
            public:
                CountCallSites();
                virtual ~CountCallSites();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            class CountCallees : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
                IRule* rule;
                gim::Mutatee* mutatee;
            public:
                CountCallees();
                ~CountCallees();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            /**
             * Expose number of exit sites
             *
             * currently no function with exitcount==0 is instrumented
             * check instrument.cpp
             * */
            class CountExits : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
                IRule* rule;
                gim::Mutatee* mutatee;
            public:
                CountExits();
                ~CountExits();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            /**
             * Expose number of enter locations to filter
             *
             */
            class CountEnter : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
                IRule* rule;
                gim::Mutatee* mutatee;
            public:
                CountEnter();
                ~CountEnter();

                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);

                virtual bool match(gim::IFunction* f);
            };

            /**
             * HasOverlay property is used to expose overlapping functions to filter
             */
            class HasOverlay : public IProperty {
            public:
                HasOverlay();
                ~HasOverlay();
                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);
                virtual bool match(gim::IFunction* f);
            };

            class RetFloatOrDouble : public IProperty {
            public:
                RetFloatOrDouble();
                ~RetFloatOrDouble();
                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* mutatee);
                virtual bool match(gim::IFunction* f);
            };


            /**
             * Check whether function is called in a loop
             * loop needs to nested at level
             */
            class CalledInLoop : public IProperty {
            private:
                gim::Mutatee* mutatee;
                unsigned int level;
                
                bool checkLevel(const string &mangledName,unsigned int currentLevel, BPatch_loopTreeNode* node);

            public:
                CalledInLoop();
                ~CalledInLoop();
                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* m);
                virtual bool match(gim::IFunction* f);
            };


            /**
             * Filter to count the number of instructions in a function
             */
            class CountInst : public IProperty {
            private:
                unsigned int minValue;
                unsigned int maxValue;
            public:
                CountInst();
                ~CountInst();
                virtual bool needsInit();
                virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement);
                virtual bool init(gim::Mutatee* m);
                virtual bool match(gim::IFunction* f);
            };


        }
    }
}

#endif	/* _IMPLPROPERTIES_H */

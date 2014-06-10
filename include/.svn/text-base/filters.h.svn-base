/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filters.h
 * @author Jan Mussler
 * @brief Defintion of filter related classes
 */

#ifndef _FILTERS_H
#define	_FILTERS_H

#include <vector>
#include <map>
#include <string>
#include <set>
#include "mutatee.h"
#include "ruletree.h"
#include "parserexceptions.h"

using namespace gi::mutatee;
using namespace std;

namespace gi {
    namespace filter {

        typedef set<IInstrumentableFunction*> filterResult;

        /**
         * Interface for Filter Elements, which are either include,exclure or a rule
         */
        class IFilterElement {
        public:

            virtual ~IFilterElement() {
            };

            /**
             * @return rule associated with element
             */
            virtual IRule* getRule() = 0;
            /**
             * @param baseSet the set of all possible functions in the mutatee
             * @param currentSet the set to be filltered, may become larger by adding functions from base set
             */
            virtual filterResult* getResult(filterResult* baseSet, filterResult* currentSet) = 0;
        };

        /**
         * Interface for filters in filterset
         */
        class IFilter {
        public:

            virtual ~IFilter() {};

            /**
             * @return name of filter
             */
            virtual string getName() = 0;

            virtual string getDescription() = 0;
            
            virtual void setDescription(string desc) = 0;

            /**
             * @return does the filter instrument anything?
             */
            virtual bool isInstrumenting() = 0;

            /**
             * @return can filter be used as start set in other filter?
             */
            virtual bool isReferenceAble() = 0;

            /**
             * get the filtered set resulting from the baseSet
             * @param baseSet starting set of functions, from which to exclude or include
             */
            virtual filterResult* getResult(filterResult* baseSet) = 0;
        };

        /**
         * Interface for a set of filters
         */
        class IFilterSet {
        public:
            virtual ~IFilterSet() {};
            
            /**
             * get an IFilter object associated with the name
             */
            virtual IFilter* getFilterByName(std::string aName) = 0;
            virtual void addFilter(std::string aName, IFilter* aFilter) = 0;
            virtual std::set<std::string> getFilterNames() = 0;
            virtual std::vector<IFilter*> getInstrumentingFilters() = 0;
        };

        /**
         * Filter Element
         *   has one rule tree, which will be used to filter set of functions
         */
        class FilterElement : public IFilterElement {
        protected:
            IRule* filterRuleTree;
        public:
            FilterElement();
            /**
             * @param aRuleTree rule tree associated with element
             */
            FilterElement(IRule* aRuleTree);

            virtual ~FilterElement();

            /**
             * @return associated rule
             */
            IRule* getRule();

            /**
             * @param baseSet functions from which include selects functions to add
             * @param currentSet result set of functions to add to, or remove from, coming from previous element
             * @return set of functions
             */
            virtual filterResult* getResult(filterResult* baseSet, filterResult* currentSet);
        };

        /**
         * Include Element
         *   gets passed the mutatee, include set will add new functions to set
         *   adds functions which match the associated rules
         */
        class IncludeElement : public FilterElement {
        public:
            /**
             * @param aRuleTree rule tree
             */
            IncludeElement(IRule* aRuleTree);
            virtual filterResult* getResult(filterResult* baseSet, filterResult* currentSet);
        };

        /**
         * Excluse Element
         *   removes functions from the set of functions passed to it
         */
        class ExcludeElement : public FilterElement {
        public:
            /**
             * @param aRuleTree associated rule Tree
             */
            ExcludeElement(IRule* aRuleTree);
            virtual filterResult* getResult(filterResult* baseSet, filterResult* currentSet);
        };

        /**
         * class to store how to instrument what
         * takes care of splitting string and saving key=values
         */
        class InstrumentSettings {
            friend class Filter;
        private:
            string loops;
            string callsites;
            string functions;
        public:
            /**
             * Takes the string argument, which has to match the following pattern:
             *   functions[=codename],loops[=codename],callsites[=codenmae]
             *
             * which specify how theses instrumentation points will be instrumented
             * where codename is an identifier from the adapter spec file
             *
             * @param how string defining what to instrument with which code
             */
            InstrumentSettings(string how);
        };

        /**
         * Implementation of filter
         */
        class Filter : public IFilter {
        private:
            string name;
            string startsetname;
            string description;
            InstrumentSettings instSettings;
            vector<IFilterElement*> filters;
            IFilterSet* parentFilterSet;

            /* additional rule where to instrument callsites */
            IRule* callSiteRule;

            /* additional rule where to instrument loops */
            IRule* loopRule;

            // inst only outer loops
            bool loopsOnlyOuter;
            std::set<std::string> loopNames;

            /**
             * @return whether this filter instruments function enter exit
             */
            bool instrumentFunctions();

            /**
             * @return whether this filter instruments any loop instrumentation point
             */
            bool instrumentLoops();

            /**
             * @return whether this filter instruments any callsite point
             */
            bool instrumentCallSites();

            /**
             * @return name of code in adapter used to instrument functions
             */
            string getFunctionCodeName();

            /**
             * @return name of code in adapter used to instrument loops
             */
            string getLoopCodeName();

            /**
             * @return name of code in adapter used to instrument callsites
             */
            string getCallSiteCodeName();

            bool checkLoopByName(std::string name);

        public:
            /**
             * @param aName name of filter
             * @param startsetName name of starset
             * @param instrument what to instrument
             * @param parent filterset which contains filter
             */
            Filter(std::string aName, std::string startsetName, std::string instrument, IFilterSet* parent);
            virtual ~Filter();

            /**
             * @return whether filter is referencable ( only one rule child )
             */
            virtual bool isReferenceAble();

            /**
             * @return does filter instrument anything?
             */
            virtual bool isInstrumenting();

            /**
             * @return name of filter
             */
            virtual string getName();

            /**
             * @return description
             */
            virtual string getDescription();

            /**
             * @param desc Description of filter
             */
            void setDescription(string desc);

            /**
             * add new element to filter
             * 
             * @param newElement new element
             */
            void addElement(IFilterElement* newElement);

            /**
             * @param rule set rule to check whether loops in this function should be instrumented
             */
            void setLoopRule(IRule* rule);

            /**
             * @param value set whether or not to instrument only outer loops
             */
            void setLoopsOnlyOuter(bool value);

            /**
             * @param rul Rule applied to called function
             */
            void setCallSiteRule(IRule* rule);

            void addLoopName(std::string name);

            /**
             * filter the input set
             *   baseSet is max set, include nodes will depend on base set
             * @param baseSet max set which will be filtered
             * @return new set of functions to instrument
             */
            virtual filterResult* getResult(filterResult* baseSet);
        };


        typedef map<std::string, IFilter*> mapStringIFilter;

        /**
         * Implementation of set of filters
         *   generated by parsing the user specified xml file
         */
        class FilterSet : public IFilterSet {
        protected:
             mapStringIFilter filters;
        public:
            FilterSet();
            virtual ~FilterSet();

            /**
             * @param aName name of filter
             * @return filter
             */
            virtual IFilter* getFilterByName(std::string aName);

            /**
             * add filter to set
             * @param aName name of filter
             * @param aFilter filter
             */
            virtual void addFilter(std::string aName, IFilter* aFilter);

            virtual set<string> getFilterNames();

            /**
             * @return list of filters that actualy do instrumentation
             */
            virtual vector<IFilter*> getInstrumentingFilters();
        };
    }
}

#endif	/* _FILTERS_H */

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   ruletree.h
 * @author Jan Mussler
 * @brief Rule tree class definitions
 */

#ifndef GI_FILTER
#define GI_FILTER

#include <vector>
#include <map>

#include "rule.h"
#include "ifunction.h"

namespace gim = gi::mutatee;

namespace gi {
    namespace filter {
        /**
         * Interface, baseclass for sets of filters combined by one opperation
         */
        class ARuleSet : public IRule {
        protected:
            std::vector<IRule*> children;
        public:
            virtual ~ARuleSet();
            void addRule(IRule* rule);
            unsigned int count();
        };

        /**
         * set of filters where all filters have to match
         */
        class AND : public ARuleSet {
        public:
            AND();
            virtual bool match(gim::IFunction* f);
        };

        /**
         * set of filters where any filter has to match
         */
        class OR : public ARuleSet {
        public:
            OR();
            virtual bool match(gim::IFunction* f);
        };

        /**
         * negate filter result
         */
        class NOT : public IRule {
        private:
            IRule* child;
        public:
            ~NOT();
            NOT();
            void setRule(IRule* rule);
            virtual bool match(gim::IFunction* f);
        };

        /**
         * Node "true" in filter spec
         * added for ease of testing
         */
        class RTrue : public IRule {
        public:
            virtual bool match(gim::IFunction* f) { return true; }
        };

        /**
         * Node "false" in filter spec
         * added for ease of testing
         */
        class RFalse : public IRule {
        public:
            virtual bool match(gim::IFunction* f) { return false; }
        };
    }
}
#endif /* GI_FILTER */

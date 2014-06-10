/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   rule.h
 * @author Jan Mussler
 * @brief Rule interface defintion
 */

#ifndef _RULE_H
#define	_RULE_H

#include "ifunction.h"

namespace gim = gi::mutatee;

namespace gi {
    namespace filter {
        /**
         * base class for all rules, all rules take a function and check if they match
         */
        class IRule {
        public:
            virtual ~IRule() {};
            virtual bool match(gim::IFunction* f) = 0;
        };
    }
}

#endif	/* _RULE_H */

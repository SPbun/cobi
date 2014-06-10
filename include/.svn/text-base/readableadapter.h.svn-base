/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   readableadapter.h
 * @author Jan Mussler
 * @brief Readable adapter interface defintion
 */

#ifndef _ADAPTER_H
#define	_ADAPTER_H

#include <string>

#include "context.h"
#include "rule.h"
#include "dependency.h"

namespace gig = gi::generator;
namespace gif = gi::filter;

namespace gi {
    namespace instrumenter {

        /**
         * Read Interface for Adapter Configuration
         **/
        class IReadableAdapterConfig {
        public:

            virtual ~IReadableAdapterConfig() {
            };

            virtual gig::IContext* createVariableContextForName(gig::IContext* parentContext, std::string name) = 0;
            virtual gig::IContext* createGlobalVariableContext(gig::IContext* parentContext) = 0;

            virtual BPatch_snippet* getInitCodeByName(gig::IContext* context, std::string name) = 0;
            virtual BPatch_snippet* getBeforeCodeByName(gig::IContext* context, std::string name) = 0;
            virtual BPatch_snippet* getAfterCodeByName(gig::IContext* context, std::string name) = 0;
            virtual BPatch_snippet* getEnterCodeByName(gig::IContext* context, std::string name) = 0;
            virtual BPatch_snippet* getExitCodeByName(gig::IContext* context, std::string name) = 0;
            virtual BPatch_snippet* getFinalizeCodeByName(gig::IContext* context, std::string name) = 0;

            virtual int getPriorityForInitCode(std::string name) = 0;

            virtual gif::IRule* getAdapterFilterRule() = 0;
            virtual vector<gi::instrumenter::Dependency> getDependencies() = 0;

            virtual std::set<std::string> getNonReturningFunctions() = 0;
            virtual bool saveAllFPRs() = 0;

            virtual std::string getFunctionForInitCode() = 0;
            virtual std::string getFunctionForFinalizeCode() = 0;

            virtual bool hasAdapterFilterRule() = 0;
            virtual bool hasInitCodeFor(std::string name) = 0;
            virtual bool hasExitCodeFor(std::string name) = 0;
            virtual bool hasEnterCodeFor(std::string name) = 0;
            virtual bool hasBeforeCodeFor(std::string name) = 0;
            virtual bool hasAfterCodeFor(std::string name) = 0;
            virtual bool hasFinalizeCodeFor(std::string name) = 0;
            virtual bool hasGlobalVariables() = 0;

            virtual bool hasAdapterInitCode() = 0;
            virtual bool hasAdapterFinalizeCode() = 0;

            virtual BPatch_snippet* getAdapterInitCode(gig::IContext* context) = 0;
            virtual BPatch_snippet* getAdapterFinalizeCode(gig::IContext* context) = 0;
        };
    }
}
#endif	/* _ADAPTER_H */

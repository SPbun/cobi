/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   adapterconfig.h
 * @author Jan Mussler
 * @brief  Adapterconfig class
 */

#ifndef _INSTRUMENTER_H
#define	_INSTRUMENTER_H

#include "BPatch.h"
#include "BPatch_point.h"
#include <string>
#include <set>
#include <map>
#include <vector>
#include "mutatee.h"
#include "generator.h"
#include "readableadapter.h"
#include "context.h"
#include "instrumentations.h"
#include "variable.h"
#include "dependency.h"


// using namespace gi::mutatee;

namespace gim = gi::mutatee;
namespace gig = gi::generator;
namespace gif = gi::filter;

using namespace std;

namespace gi {
    namespace instrumenter {

        namespace ex {

            /**
             * Base class for instrumenter exceptions
             */
            class InstrumenterException {
            };

            /**
             * Exception if code used in filter is not defined ( missspelled or missing in spec )
             */
            class CodeNotDefindedException : public InstrumenterException {
            };
        }

        typedef set<Variable> setVariable;

        typedef map<std::string, gig::IExpression*> mapStringIExpression;
        typedef std::map<std::string, int> mapStringPriority;

        /**
         * AdapterConfig
         *
         *   stores all code specified in the adapter config file
         *   stores variable that are associated with code sections
         *   stores dependencies
         */
        class AdapterConfig : public IReadableAdapterConfig {
        private:
            vector<Dependency> depList;

            // set of global variables from adapter spec, instantiated once
            setVariable globalVariables;

            // associates code name to list of variables
            // these variables will be instantiated per context (function,loop,callsite)
            map<string, setVariable > varListPerName;

            mapStringIExpression initCode;
            mapStringIExpression beforeCode;
            mapStringIExpression enterCode;
            mapStringIExpression exitCode;
            mapStringIExpression afterCode;
            mapStringIExpression finalizeCode;


            /*             
             need to store priorities, different types of init code may need defined order
             see e.g. TAU enable dyninst before trace register region calls
             */
            mapStringPriority initPriorities;


            gif::IRule* adapterFilterRule;

            std::set<std::string> nonReturningFunctions;
            bool mSaveAllFPRs;
            std::string mFunctionForInit;
            std::string mFunctionForFinalize;

            gig::IExpression* mAdapterInitCode;
            gig::IExpression* mAdapterFinalizeCode;

        public:
            AdapterConfig();
            ~AdapterConfig();

            /**
             * create all variables stored under name
             */
            gig::IContext* createVariableContextForName(gig::IContext* parentContext, string name);

            gig::IContext* createGlobalVariableContext(gig::IContext* parentContext);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getInitCodeByName(gig::IContext* context, string name);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getBeforeCodeByName(gig::IContext* context, string name);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getAfterCodeByName(gig::IContext* context, string name);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getEnterCodeByName(gig::IContext* context, string name);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getExitCodeByName(gig::IContext* context, string name);

            /**
             * query the configuration for the named snippet, using given context to evaluate
             */
            BPatch_snippet* getFinalizeCodeByName(gig::IContext* context, string name);

            /**
             * @param name
             * @return Priority for init code to insert in correct order
             */
            int getPriorityForInitCode(std::string name);

            /**
             * return adapter filter rule, used to exclude all functions belonging to the adapter
             */
            gif::IRule* getAdapterFilterRule();

            /**
             * @return whether adapter has a rule to exclude adapter functions
             */
            bool hasAdapterFilterRule();

            /**
             * @return is there any code specified for the given name
             */
            bool hasExitCodeFor(string name);

            /**
             * @return is there any code specified for the given name
             */
            bool hasEnterCodeFor(string name);

            /**
             * @return is there any code specified for the given name
             */
            bool hasBeforeCodeFor(string name);

            /**
             * @return is there any code specified for the given name
             */
            bool hasAfterCodeFor(string name);

            bool hasInitCodeFor(string name);
            bool hasFinalizeCodeFor(string name);

            /**
             * add named code to this configuration
             *
             * @param e expression root element
             * @param name expression by name
             * @param prio order of inserts into binary, lower equals earlier execution
             */
            void addInitExpression(gig::IExpression* e, string name, int prio = 10);

            /**
             * add named code to this configuration
             *   used for initialisation
             */
            void addFinalizeExpression(gig::IExpression* e, string name);

            /**
             * add named code to this configuration
             *   used at before points ( loop and callsites )
             */
            void addBeforeExpression(gig::IExpression* e, string name);

            /**
             * add named code to this configuration
             *   used at after poitns ( loop and callsites )
             */
            void addAfterExpression(gig::IExpression* e, string name);

            /**
             * add named code to this configuration
             *   used at function or loop body entry
             */
            void addEnterExpression(gig::IExpression* e, string name);

            void setAdapterFilterRule(gif::IRule* rule);
            /**
             * add named code to this configuration
             *   used at function or loop body exit
             */
            void addExitExpression(gig::IExpression* e, string name);

            /**
             * add variable list associated with named code to configuration
             */
            void addVariableList(setVariable list, string name);

            void setGlobalVariableList(setVariable list);

            bool hasGlobalVariables();

            /**
             * add a dependency to the config
             * dependencies are dynamic libraries which will be added to the mutatee
             */
            void addDependency(Dependency dep);

            vector<Dependency> getDependencies();

            void addNonReturningFunction(std::string funcName);

            std::set<std::string> getNonReturningFunctions();

            void setSaveAllFPRs(bool value);

            bool saveAllFPRs();

            void setFunctionForInitCode(std::string name);
            std::string getFunctionForInitCode();
            
            void setFunctionForFinalize(std::string name);
            std::string getFunctionForFinalizeCode();

            bool hasAdapterInitCode();
            bool hasAdapterFinalizeCode();

            void setAdapterFinalizeCode(gig::IExpression* code);
            void setAdapterInitCode(gig::IExpression* code);

            /**
             *
             * @return Init code snippet that will be inserted in mFunctionForInit
             */

            BPatch_snippet* getAdapterInitCode(gig::IContext* context);

            /**
             *
             * @return Finalize code snippet that will be inserted in mFunctionForFinalize
             */
            BPatch_snippet* getAdapterFinalizeCode(gig::IContext* context);
        };

        /**
         * instrument a mutatee ( stores his own set of instrumentable functions )
         *   adapter contains specification of named code snippets
         */
        void instrument(gim::Mutatee* mutatee, IReadableAdapterConfig* adapter);
    }
}

#endif	/* _INSTRUMENTER_H */

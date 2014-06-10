/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   .h
 * @author Jan Mussler
 * @brief
 */

#ifndef _MUTATEE_H
#define	_MUTATEE_H

#include <string>
#include <vector>

#include "BPatch.h"
#include "BPatch_function.h"
#include "BPatch_module.h"
#include "BPatch_loopTreeNode.h"
#include "BPatch_flowGraph.h"

#include "rule.h"
#include "dependency.h"
#include "readableadapter.h"

using namespace std;

namespace gii = gi::instrumenter;
namespace gim = gi::mutatee;
namespace gig = gi::generator;

namespace gi {
    namespace mutatee {

        /**
         * Decorating class for dyninst bpatch_function
         */
        class Function : public IFunction {
        private:
            string name;
            string functionname;
            BPatch_function* f;
            Function() {};
        public:
            Function(BPatch_function* aF);
            virtual ~Function();
            virtual string getName();
            virtual string getFunctionName();
            virtual string getModuleName();
            virtual string getClassName();
            virtual string getNameSpace();
            virtual string getMangledName();
            virtual vector<IFunction*> getSubCalls(bool incPointerCalls = false);
            BPatch_function* getBPFunction();
            
            bool isExternalFunc();
            bool isVirtualFuncObj();
        };

        /**
         *
         * @param BPatch_point point where instrumentation is inserted
         * @return Function Object representing the function including the point
         **/
        IFunction* getFunctionToPoint(BPatch_point* p);

        /**
         * Function object for subroutine call that can not be resolved ( external librarys )
         */
        class ExternalFunction : public IFunction {
        private:
            string name;
        public:
            ExternalFunction(string aName);
            virtual ~ExternalFunction();
            virtual string getName();
            virtual string getFunctionName();
            virtual string getMangledName();
            virtual string getClassName();
            virtual string getNameSpace();
            virtual string getModuleName();
            virtual vector<IFunction*> getSubCalls(bool incPointerCalls = false);
            BPatch_function* getBPFunction();

            bool isExternalFunc();
            bool isVirtualFuncObj();
        };

        /* used as object for callsites that make calls by function pointer
           e.g., virtual functions and c function pointer calls */
        class VirtualFunction : public IFunction {
        public:
            VirtualFunction();
            virtual ~VirtualFunction();
            string getName();
            string getFunctionName();
            string getMangledName();
            string getClassName();
            string getNameSpace();
            string getModuleName();
            virtual vector<IFunction*> getSubCalls(bool incPointerCalls = false);
            BPatch_function* getBPFunction();

            bool isExternalFunc();
            bool isVirtualFuncObj();
        };

        class Parameter {
        private:
            string name;
        public:
            Parameter(string aName);
            string getName();
        };

        /**
         * Decorator for BPatch_Module
         */
        class Module {
        private:
            BPatch_module* m;
        public:
            Module();
            Module(BPatch_module* m);
            ~Module();
            string getName();
            vector<Function>* getFunctions();
        };


        /* Loops */
        class LoopTreeNode;

        /**
         * TreeNode
         */
        class TreeNode {
        protected:
            BPatch_loopTreeNode* node;
        public:
            TreeNode();
            vector<LoopTreeNode> getChildren();
            BPatch_loopTreeNode* getBPNode();
        };

        /*
         * Loop Tree
         *
         */
        class LoopTree : public TreeNode {
        public:
            LoopTree(Function f);
        };

        /**
         *  LoopTreeNode
         */
        class LoopTreeNode : public TreeNode {
        public:
            string getName();
            LoopTreeNode(BPatch_loopTreeNode* bpNode);
        };

        class Mutatee;

        /**
         * Interface for instrumentable functions
         */
        class IInstrumentableFunction {
        public:
            virtual Function* getFunction() = 0;
            virtual bool addLoopInstrumentation(BPatch_loopTreeNode* loopNode, string codeName) = 0;
            virtual bool addCallSiteInstrumentation(BPatch_point* point, string codeName) = 0;
            virtual bool addFunctionInstrumentation(string name) = 0;

            virtual bool isMarkedForInstrumentation() = 0;

            virtual void storeContext(string instCodeName, gig::IContext* context) = 0;
            virtual gig::IContext* getContext(string instCodeName) = 0;
            virtual gig::IContext* getFunctionContext() = 0;

            virtual bool instrument(Mutatee* mutatee, gii::IReadableAdapterConfig* adapter, gig::IContext* context) = 0;
            virtual ~IInstrumentableFunction() {};
        };

        class SourceLine {
        public:
            int line;
            string file;
            SourceLine(int l, string f);
        };

        namespace graph {
            class CallGraph;
        }

        typedef std::map<int,vector<BPatch_snippet*> > mapPrioToSnippets;

        /**
         * Mutatee class
         */
        class Mutatee {
        private:            
            BPatch_binaryEdit* addressSpace;
            BPatch_image* image;

            vector<Function*>* baseFunctions;
            set<IInstrumentableFunction*>* baseSet;
            graph::CallGraph* callGraph;

            /**
             * rule applied before generating base set
             * should exclude all functions of the adapter, which should not be instrumented
             */
            gi::filter::IRule* adapterRule;

            BPatch_function* initFunction;
            BPatch_function* finalizeFunction;

            BPatch_point* initInsertPoint;
            vector<BPatch_point*>* finalizeInsertPoints;

            mapPrioToSnippets initSnippets;
            
        public:
            Mutatee();
            Mutatee(BPatch_binaryEdit* as, BPatch_image* im);
            ~Mutatee();

            vector<Module>* getModules();

            /**
             * Gives you a vector of all functions in the mutatee binary
             *
             * do not delete the returned vector, mutatee takes care of that!!
             *
             * @return all functions
             */
            vector<Function*>* getAllFunctions();

            void setAdapterRule(gi::filter::IRule* rule);

            /**
             * do not delete the returned set, mutatee takes care of that!!
             *
             * @return set of instrumentable functions
             */
            set<IInstrumentableFunction*>* getAllInstrumentableFunctions();

            SourceLine getFirstSourceLineOfAddress(unsigned long address);

            BPatch_function* getFunction(std::string name);
            
            BPatch_variableExpr* createVariable(int size);
            BPatch_variableExpr* createVariable(string typeName);

            void insertCode(vector<BPatch_point*> points,BPatch_snippet* snippet);
            void insertCode(vector<BPatch_point*> points,BPatch_snippet* snippet,BPatch_callWhen w,BPatch_snippetOrder o);

            void insertInitCode(BPatch_snippet* snippet, int prio = 10);
            void insertInitCode(BPatch_snippet* snippet,BPatch_callWhen w,BPatch_snippetOrder o, int prio = 10);
            
            void insertFinalizeCode(BPatch_snippet* snippet);
            void insertFinalizeCode(BPatch_snippet* snippet,BPatch_callWhen w,BPatch_snippetOrder o);

            void setNameOfFunctionWhereToPlaceInit(string name);
            void setNameOfFunctionWhereToPlaceFinalize(string name);

            void addDependency(gii::Dependency dep);

            bool save(string fileName);

            graph::CallGraph& getCallGraph();

            BPatch_addressSpace* getAddressSpace() { return addressSpace; };
        };

        class DynFactroy {
        public:
            /**
             * Open a target binary
             *  flags regarding opening dependencies, saveing FP registers and tramp guard
             *  must at this point be set through the Cobiconfig::getCfg() Config object
             *
             * @return Mutatee representing the target binary
             */
            static Mutatee* getMutatee(BPatch& bp, std::string fileName, gii::IReadableAdapterConfig* config );
        };
    }
}

#endif	/* _MUTATEE_H */

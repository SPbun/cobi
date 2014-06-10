/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   generator.h
 * @author Jan Mussler
 * @brief
 */

#ifndef GI_GENERATOR
#define GI_GENERATOR

#include <string>
#include <map>
#include <vector>

#include "BPatch.h"
#include "BPatch_snippet.h"

#include "context.h"

using namespace std;

namespace gi {
    namespace generator {

        namespace ex {

            class GeneratorException {
            public:
            };

            class UnknownVariable : public GeneratorException {
            public:
            };

            class UnknownFunction : public GeneratorException {
            };
            
        }

        /**
         * Interface for expression nodes
         */
        class IExpression {
        public:
            virtual BPatch_snippet* getSnippet(IContext* c) = 0;
            virtual ~IExpression();
        };

        class IBoolExpression : virtual public IExpression {
        public:
            virtual BPatch_boolExpr* getBoolSnippet(IContext* c) = 0;
            virtual BPatch_snippet* getSnippet(IContext* c) = 0;

            virtual ~IBoolExpression() {
            };
        };

        /**
         * Dyninst NullExpr
         */
        class NullOp : public IExpression {
        public:
            virtual BPatch_snippet* getSnippet(IContext* c);
            NullOp();
            ~NullOp();
        };

        /**
         * Implementation of code sequence
         */
        class Sequence : public IExpression {
        private:
            vector<IExpression*> statements;
        public:
            Sequence();
            ~Sequence();
            void addStatement(IExpression* e);
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * Function Call
         */
        class FunctionCall : public IExpression {
        private:
            vector<IExpression*> parameters;
            string functionName;
        public:
            FunctionCall(string name);
            ~FunctionCall();
            void addParameter(IExpression* e);
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * Binary operation
         */
        class BinaryOp : virtual public IExpression {
        protected:
            IExpression* leftOperand;
            IExpression* rightOperand;
        public:
            BinaryOp(IExpression* leftOp, IExpression* rightOp);
            ~BinaryOp();
            virtual BPatch_snippet* getSnippet(IContext* c) = 0;
        };

        class BoolExpression : public BinaryOp, public IBoolExpression {
        private:

            enum boolOp {
                EQUALS, NOT_EQUALS, GREATER, LESS, LESS_EQUAL, GREATER_EQUAL
            };
            boolOp op;
        public:
            BoolExpression(std::string relOp, IExpression* lOp, IExpression* rOp);
            BPatch_boolExpr* getBoolSnippet(IContext* c);
            BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * Assignment operation
         */
        class Assignment : public BinaryOp {
        public:
            Assignment(IExpression* leftOp, IExpression* rightOp);
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * variable expression
         * will look up context and return variable snippet
         */
        class Variable : public IExpression {
        private:
            string name;
        public:
            /**
             * @param aName Variable name
             */
            Variable(string aName);

            /**
             * @return variable snippet from context
             */
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        class FunctionParam : public IExpression {
        private:
            int paramNumber;
        public:
            FunctionParam(string aName);
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * Constant expression base class
         */
        class Constant : public IExpression {
        public:
            /**
             * create constant string expression
             */
            static Constant* createConstant(string s);

            /**
             * create constant int expression
             */
            static Constant* createConstant(int i);

            virtual BPatch_snippet* getSnippet(IContext* c) = 0;

            virtual string getStringValue(IContext* c) = 0;
        };

        class ConcatConstants : public Constant {
        private:
            Constant* leftOp;
            Constant* rightOp;
            BPatch_snippet* snippet;
        public:
            ConcatConstants(Constant* lo, Constant* ro);
            void setLeftOp(Constant* lo);
            void setRightOp(Constant* ro);
            virtual string getStringValue(IContext* c);
            virtual BPatch_snippet* getSnippet(IContext* c);
        };

        class ContextConstant : public Constant {
        private:
            string name;
        public:
            ContextConstant(string aName);
            string getStringValue(IContext* c);
            BPatch_snippet* getSnippet(IContext*);
        };

        /**
         * constant integer expression
         */
        class IntConstant : public Constant {
            friend class Constant;
        private:
            int value;
            IntConstant(int i);
        public:
            string getStringValue(IContext* c);
            BPatch_snippet* getSnippet(IContext* c);
        };

        /**
         * constant string expression
         */
        class StrConstant : public Constant {
            friend class Constant;
        private:
            string value;
            StrConstant(string s);
        public:
            string getStringValue(IContext* c);
            BPatch_snippet* getSnippet(IContext* c);
        };

        class Conditional : public IExpression {
        private:
            IBoolExpression* boolExp;

            /**
             * True expression
             */
            IExpression* tProg;

            /**
             * False expression
             */
            IExpression* fProg;
        public:

            /**
             * @param bExp boolExpression to evaluate
             * @param tProg expression for the true case
             * @param fProg expression for the false case
             */
            Conditional(IBoolExpression* bExp, IExpression* tProg, IExpression* fProg);
            ~Conditional();
            BPatch_snippet* getSnippet(IContext* c);
        };

        class Term : public BinaryOp {
        public:

            enum binOperation {
                PLUS, MINUS, DIVIDE, MULTIPLY
            };
        private:
            binOperation op;
        public:
            Term(string binOp, IExpression* lo, IExpression* ro);
            BPatch_snippet* getSnippet(IContext* c);
        };
    }
}
#endif

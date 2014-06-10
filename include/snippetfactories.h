/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   snippetfactories.h
 * @author Jan Mussler
 * @brief  Snippetfactoy definition
 */

#ifndef SNIPPETFACTORIES_H
#define	SNIPPETFACTORIES_H

#include "codeparser.h"
#include "generator.h"

using namespace BOOST_SPIRIT_CLASSIC_NS;

namespace gi {
    namespace generator {
        namespace parser {

            /**
             * interface for snippet factory
             */
            class ISnippetFactory {
            public:

                virtual ~ISnippetFactory() {
                };
                virtual IExpression* evaluateNode(tree_node<node_val_data<> >* node) = 0;
                std::string getValueOfNode(tree_node<node_val_data<> >* node);
            };

            /**
             * parser for code in instrumentation specification
             */
            class CodeParser {
                friend class CodeParserTest;
            public:

                enum SnippetFactories {
                    DEMO, GI
                };
            private:
                static codegrammar g;


                /**
                 * return parser AST ( used in CodeParserTest )
                 * @param code code to be parsed
                 * @return AST of IExpression nodes
                 */
                tree_parse_info<> getTreeInfoAST(std::string code);

                /**
                 * @param type type of factory
                 * @return snippet factory
                 */
                ISnippetFactory* getSnippetFactory(SnippetFactories type);
            public:

                CodeParser();
                ~CodeParser();

                /**
                 * @param id id of rule
                 * @return name of rule
                 */
                static std::string getRuleNameToID(parser_id id);

                /**
                 * return AST of IExpression nodes used in the code generator
                 * @param code code to be parsed
                 * @return AST of IExpression nodes
                 */
                IExpression* getIExprAST(std::string code);
            };

            /**
             * demo implementation of snippetfactory
             * produces only text output, to test parser
             */
            class DemoSnippetFactory : public ISnippetFactory {
            private:
                codegrammar &g;
                int level;
            public:
                IExpression* evaluateNode(tree_node<node_val_data<> >* node);
                DemoSnippetFactory(parser::codegrammar &g);
                ~DemoSnippetFactory();
            };

            /**
             * snippet factory implementation which produces
             * expression node tree for code passed to parser
             */
            class SnippetFactory : public ISnippetFactory {
            private:
                codegrammar &g;
            public:
                /**
                 * evaluate node, return expression and descent into tree
                 * @param node node in boost ast tree
                 * @return tree of gi internal expression nodes
                 */
                IExpression* evaluateNode(tree_node<node_val_data<> >* node);

                /**
                 * evaluate a node representing a boolean expression ( necessary due to type restriction )
                 *
                 * IBoolExpression => BPatch_boolExpr for BPatch_ifExpr
                 *
                 * @param node
                 * @return tree, with root IBoolExpression type node
                 */
                IBoolExpression* evaluateBoolExpressionNode(tree_node<node_val_data<> >* node);

                /*
                 * evaluate constant nodes, children of "." operator
                 */
                Constant* evaluateConstantNode(tree_node<node_val_data<> >* node);


                /**
                 * @param g grammar used for parsing code
                 */
                SnippetFactory(parser::codegrammar &g);
                ~SnippetFactory();
            };
        }
    }
}
#endif	/* SNIPPETFACTORIES_H */

/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   codeparser.cpp
 * @author Jan Mussler
 * @brief Implements the code parser class
 */

#include "snippetfactories.h"

using namespace gi::generator::parser;
using namespace gi::generator;

using namespace BOOST_SPIRIT_CLASSIC_NS;

CodeParser::CodeParser() {
}

CodeParser::~CodeParser() {
}

codegrammar CodeParser::g;

/**
 * return name associated with rule id
 */
std::string CodeParser::getRuleNameToID(parser_id id) {
    if (id == g.assignmentID) {
        return std::string("Assignment");
    }
    if (id == g.stringLiteralID) {
        return std::string("String");
    }
    if (id == g.intLiteralID) {
        return std::string("Integer");
    }
    if (id == g.constantID) {
        return std::string("Constant");
    }
    if (id == g.expressionID) {
        return std::string("Expresssion");
    }
    if (id == g.functionID) {
        return std::string("Function");
    }
    if (id == g.identifierID) {
        return std::string("Identifier");
    }
    if (id == g.paramID) {
        return std::string("Parameter");
    }
    if (id == g.paramlisteID) {
        return std::string("Parameterlist");
    }
    if (id == g.programID) {
        return std::string("Program");
    }
    if (id == g.statementID) {
        return std::string("Statement");
    }
    if (id == g.boolExpressionID) {
        return std::string("bool Expression");
    }
    if (id == g.conditionalID) {
        return std::string("IF Statement");
    }
    if (id == g.termID) {
        return std::string("Term Statement");
    }
    if (id == g.concatConstStrID) {
        return std::string("Concat Const Strings");
    }
    if (id == g.funcParID) {
        return std::string("Function Param Access");
    }
    if (id == g.contextVarID) {
        return std::string("Context Variable");
    }


    //TODO: Exception?
    return std::string("UNKNOWN NODE!!");
}

/**
 * Demo parser only generates output to std::out
 * GI produces AST of nodes from gi::generator classes
 */
ISnippetFactory* CodeParser::getSnippetFactory(SnippetFactories type) {
    switch (type) {
        case DEMO: return new DemoSnippetFactory(g);
            break;
        case GI: return new SnippetFactory(g);
            break;
    }
    throw "ERROR: no snippet factory - codeparser.cpp 66";
}

/**
 * Remove \n \t " " from the input string
 *
 * @param code
 * @return code minus whitespace in front and at the end
 */
std::string removeTrailingWhiteSpaceFrom(string code) {    
    while (code.find("\n") == 0 ||
            code.find("\t") == 0 ||
            code.find(" ") == 0)
        code.replace(0, 1, "");

    while (code.rfind("\n") == code.size() - 1 ||
            code.rfind("\t") == code.size() - 1 ||
            code.rfind(" ") == code.size() - 1)
        code.replace(code.size() - 1, 1, "");

    return code;
}

/**
 * get the internal tree_parse_info structure which is created by the boost parser
 */
tree_parse_info<> CodeParser::getTreeInfoAST(std::string code) {
    typedef node_val_data_factory<std::string> factory_t;

    code = removeTrailingWhiteSpaceFrom(code);

    return ast_parse(code.c_str(), CodeParser::g, space_p);
}

/**
 * parse code and return gi interal AST which can be evaluated to Dyninst snippet
 */
IExpression* CodeParser::getIExprAST(std::string code) {
    // parse code

    tree_parse_info<> ast = getTreeInfoAST(code);

    if (!ast.full) {
        return 0;
    }

    // get IExpression* node factory
    ISnippetFactory* sf = getSnippetFactory(GI);
    assert(sf != 0);

    // transform spirit tree to tree of IExpression* nodes
    return sf->evaluateNode(&ast.trees[0]);
}

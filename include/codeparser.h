/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   codeparser.h
 * @author Jan Mussler
 * @brief Boost::Spirit grammar definition
 */

#ifndef _CODEPARSER_H
#define	_CODEPARSER_H

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_tree_to_xml.hpp>
#include <boost/spirit/include/classic_confix.hpp>
#include <boost/spirit/include/classic_escape_char.hpp>

#include <map>
#include <string>

using namespace BOOST_SPIRIT_CLASSIC_NS;

namespace gi {
    namespace generator {

        namespace ex {
            class ParserException {
                
            };

            class SyntaxError : public ParserException {
                
            };
        }

        namespace parser {

            /**
             * grammar used to parse specified code which will be inserted at instrumentation points
             *
             */
            struct codegrammar : public grammar<codegrammar> {
                static const int programID = 1;
                static const int statementID = 2;
                static const int identifierID = 3;
                static const int functionID = 4;
                static const int assignmentID = 5;
                static const int paramlisteID = 6;
                static const int paramID = 7;
                static const int expressionID = 8;
                static const int constantID = 9;
                static const int stringLiteralID = 10;
                static const int intLiteralID = 11;
                static const int conditionalID = 12;
                static const int boolExpressionID = 13;
                static const int termID = 14;
                static const int contextVarID = 15;
                static const int funcParID = 16;
                static const int concatConstStrID = 17;
                static const int commentID = 18;

                template <typename ScannerT>
                        struct definition {

                    /**
                     * gen_pt_node_d makes the parser generate an extra node for the matching rule, e.g. a node for the rule function, otherwise there would only be the identifier node.
                     *
                     * http://www.boost.org/doc/libs/1_35_0/libs/spirit/index.html
                     */

                    definition(codegrammar const& /*self*/) {

                        // alphanum (alphanum)*
                        identifier = lexeme_d[leaf_node_d[((alpha_p|'_') >> *(alnum_p|'_'))]];
                        // identifier = leaf_node_d[((alpha_p|'_') >> *(alnum_p|'_')) | ( '@' >> (alpha_p) >> *(alnum_p) >> '@' )];

                        // access paramters given to the instrumented function
                        /* @1..9@ */
                        funcParVar = lexeme_d[leaf_node_d[( '@' >> (ch_p("1") | ch_p("2") | ch_p("3") | ch_p("4") | ch_p("5") | ch_p("6") | ch_p("7") | ch_p("8") | ch_p("9") ) >> '@' )]];

                        /* @functionname@ ... */
                        //contextVar = lexeme_d[leaf_node_d[( '@' >> (alpha_p) >> *(alnum_p) >> '@' )]];
                        contextVar = lexeme_d[leaf_node_d[str_p("@FILE@") | str_p("@LINE@") | str_p("@ROUTINE@") | str_p("@LOOP@") | str_p("@ID@") | str_p("@INTID@") | str_p("@CALLEDROUTINE@") | str_p("@FULLLOOP@") | str_p("@ADDRESS@") | str_p("@ROUTINEID@")]];

                        // param ::= function | identifier | constant | funcParVar | contextVar
                        param = gen_pt_node_d[function] | identifier | constant | funcParVar | contextVar;

                        // paramliste ::= param (, param )* | epsilon
                        // paramliste = infix_node_d[(param >> *(ch_p(",") >> param))] | epsilon_p;
                        paramliste = (param >> *(ch_p(",") >> param)) | epsilon_p;
                        
                        // function := identifier ( paramliste )
                        function = identifier >> discard_node_d[ch_p("(")] >> gen_pt_node_d[paramliste] >> discard_node_d[ch_p(")")];

                        // any integer, dissable whitespace skipping by lexeme_d
                        intLiteral = leaf_node_d[lexeme_d[+int_p]];


                        stringLiteral = leaf_node_d[confix_p('"', *c_escape_ch_p, '"')];
                                //discard_node_d[ch_p("\"")] >> leaf_node_d[lexeme_d[*(alnum_p|'_'|'-'|'%'|'.'|'?'|':'|' ')]] >> discard_node_d[ch_p("\"")];

                        // constant ::= integer | string | concat of const str
                        constant = intLiteral | concatConstStr | stringLiteral;

                        // expression ::= identifier | constant
                        expression = identifier | constant;

                        concatConstStr = ( stringLiteral | contextVar ) >> *(root_node_d[ch_p(".")] >> concatConstStr);

                        // kein punkt vor strich
                        // term = "(" term ")" ((+-*/) term)* | ( function | expression ) ((+-*/) term)*
                        term =  ( discard_node_d[str_p("(")] >> term >> discard_node_d[str_p(")")] >> *( root_node_d[str_p("+")||str_p("*")||str_p("-")||str_p("/")] >> term ) ) | ( ( gen_pt_node_d[function] | expression ) >> *( root_node_d[str_p("+")||str_p("*")||str_p("-")||str_p("/")] >> term ) );

                        // assignment ::= identifier = function | expression
                        assignment = identifier >> discard_node_d[ch_p("=")] >> (gen_pt_node_d[function] | term); //  gen_pt_node_d[function | expression ];

                        // function|expression == != < > function|expression
                        boolExpression = boolExpressionHelper >> *(root_node_d[(str_p("||") || str_p("&&"))] >> boolExpression);
                        boolExpressionHelper =  (gen_pt_node_d[function] || expression) >> root_node_d[(str_p("==") || str_p("!=") || str_p("<=") || str_p(">=") || str_p("<") || str_p(">"))] >> (gen_pt_node_d[function] || expression);

                        // if ( boolExpr ) { program }
                        conditional = discard_node_d[str_p("if")] >>  discard_node_d[ch_p("(")] >> boolExpression >>  discard_node_d[ch_p(")")] >>  discard_node_d[ch_p("{")] >> program >>  discard_node_d[ch_p("}")] >> !( discard_node_d[str_p("else")] >> discard_node_d[ch_p("{")] >> program >> discard_node_d[ch_p("}")] ) ;

                        comment = comment_p("//") | comment_p("/*","*/");

                        // statement ::= assignment | function
                        statement = gen_pt_node_d[ conditional | assignment | function ];

                        // program ::= statement; (statement ; )*
                        program = ((statement >> discard_node_d[ch_p(';')])|discard_node_d[comment]) >> *((statement >> discard_node_d[ch_p(';')])|discard_node_d[comment]);

                    }

                    rule<ScannerT, parser_context<>, parser_tag<identifierID> > identifier;
                    rule<ScannerT, parser_context<>, parser_tag<contextVarID> > contextVar;
                    rule<ScannerT, parser_context<>, parser_tag<funcParID> > funcParVar;
                    rule<ScannerT, parser_context<>, parser_tag<paramlisteID> > paramliste;
                    rule<ScannerT, parser_context<>, parser_tag<paramID> > param;
                    rule<ScannerT, parser_context<>, parser_tag<assignmentID> > assignment;
                    rule<ScannerT, parser_context<>, parser_tag<functionID> > function;
                    rule<ScannerT, parser_context<>, parser_tag<stringLiteralID> > stringLiteral;
                    rule<ScannerT, parser_context<>, parser_tag<intLiteralID> > intLiteral;
                    rule<ScannerT, parser_context<>, parser_tag<constantID> > constant;
                    rule<ScannerT, parser_context<>, parser_tag<termID> > term;
                    rule<ScannerT, parser_context<>, parser_tag<expressionID> > expression;
                    rule<ScannerT, parser_context<>, parser_tag<boolExpressionID> > boolExpression;
                    rule<ScannerT, parser_context<>, parser_tag<boolExpressionID> > boolExpressionHelper;
                    rule<ScannerT, parser_context<>, parser_tag<concatConstStrID> > concatConstStr;
                    rule<ScannerT, parser_context<>, parser_tag<conditionalID> > conditional;
                    rule<ScannerT, parser_context<>, parser_tag<statementID> > statement;
                    rule<ScannerT, parser_context<>, parser_tag<commentID> > comment;
                    rule<ScannerT, parser_context<>, parser_tag<programID> > program;

                    rule<ScannerT, parser_context<>, parser_tag<programID> > const&
                            start() const {
                        return program;
                    }
                };
            };            
        }
    }
}


#endif	/* _CODEPARSER_H */

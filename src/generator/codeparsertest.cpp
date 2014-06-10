/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   codeparsertest.cpp
 * @author Jan Mussler
 * @brief Implementation of code parser and grammar test cases
 */

#include "codeparsertest.h"

using namespace gi::generator::parser;
using namespace gi::generator;

using namespace BOOST_SPIRIT_CLASSIC_NS;

CodeParserTest::CodeParserTest() {
}

CodeParserTest::~CodeParserTest() {
}

bool CodeParserTest::test(std::string code, bool result) {
    CodeParser p;
    tree_parse_info<> info = p.getTreeInfoAST(code);

    if (result && info.full) {
        std::cout << "ok (+) \n";
        return true;
    }
    else if (!result && !info.full) {
        std::cout << "ok (-) \n";
        return true;
    }
    else {
        std::cout << "fehler: " << code << "\n";
        return false;
    }
}

void CodeParserTest::syntaxTest() {

    std::cout << "=== Testing Code Syntax ===\n";

    std::string code = "a = 123;";
    test(code, true);

    code = "a = 123 + 123;";
    test(code, true);

    code = "a = (123 + 123);";
    test(code, true);

    code = "a = (123 + 123) + 33;";
    test(code, true);
    
    code = "a = 123 + f();";
    test(code, true);


    code = "foo(\"\");";
    test(code,true);
    
    code = "foo(\"\"\");";
    test(code,false);

    code = "foo(\"m\n\tm\");";
    test(code,true);

    code = "foo(\"m\n\"\tm\");";
    test(code,false);

    code = "// blubb";
    test(code,true);

    code = "// blubb";
    test(code,true);

    code = "// blubb\n";
    test(code,true);

    code = "f(); // blubb";
    test(code,true);

    code = "f(); // blubb\ng();";
    test(code,true);

    code = " @filename@.@functionname@.@id@;";
    test(code,false);

    code = "if(a) {b = \"c\";};";
    test(code,false);

    code = "if(a==b) {b = \"c\";};";
    test(code,true);

    code = "foo(@ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(@ROUTINE@ . @ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(@ROUTINE@.\"a\",@1@,@2@);";
    test(code,true);

    code = "foo(@ROUTINE@.\"a\".\"bc\".@ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(\"a\" .@ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(\" a\".@ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(\"a\".@ROUTINE@,@1@,@2@);";
    test(code,true);

    code = "foo(\"a\".@ROUTINE@.\"b\",@1@,@2@);";
    test(code,true);

    code = " @functionname@.123;";
    test(code,false);

    // add "." operation to concat strings in instrumenter
    code = "regregion(@ROUTINE@.@ID@);";
    test(code,true);

    code = "regregion(@ROUTINE@.@2@);";
    test(code,false);
    
    code = "regregion(@ROUTINE@.\"-\".@LOOP@);";
    test(code,true);

    code = "a = \"abc\";";
    test(code, true);

    code = "f();";
    test(code, true);

    code = " f();";
    test(code, true);

    code = "blubb = af();";
    test(code, true);

    code = "blubb=af();";
    test(code, true);

    code = "f(a);";
    test(code, true);

    code = "f(a,b);";
    test(code, true);

    code = "f(123);";
    test(code, true);

    code = "f(1,23);";
    test(code, true);

    code = "f(\"1a\");";
    test(code, true);

    code = "f(\"1a\",\"blubb\");";
    test(code, true);

    code = "f(\"1a\",23);";
    test(code, true);

    code = "f(1,ab);";
    test(code, true);

    code = "f(g(c),b);";
    test(code, true);

    code = "f(1,ab);";
    test(code, true);

    code = "f(@ROUTINE@);";
    test(code, true);

    code = "a = @ROUTINE@;";
    test(code,true);

    code = "a = 123;";
    test(code,true);

    code = "a = \"abc\";";
    test(code,true);

    code = "a = \"a_bc\";";
    test(code,true);

    code = "EPIK_User_start(@ROUTINE@,@FILE@,@LINE@);";
    test(code,true);

    code = "EPIK_User_end(@ROUTINE@,@FILE@,@LINE@);";
    test(code,true);

    code = "f(@functionname);";
    test(code,false);

    // test fehlerhafte syntax

    code = "a = ;";
    test(code, false);

    code = "a(,);";
    test(code, false);

    code = "a(,b);";
    test(code, false);

    code = "= f();";
    test(code, false);

    code = "f(1,23a);";
    test(code, false);

    code = "f(1a,23);";
    test(code, false);

    code = "a = \"acbd";
    test(code, false);

    code = "b = dec\"";
    test(code, false);

    code = "b()c()d;";
    test(code, false);

    code = "b(c()d);";
    test(code, false);

    code = "b b();";
    test(code, false);

    code = "aa = a a;";
    test(code, false);

    code = "a a = aa;";
    test(code, false);

    code="if(da_return()==1) { da_enter(@ROUTINE@,@FILE@,\"return false\"); };";
    test(code,true);

    code="if(da_return()==1) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,true);

    code="if(da_return()==1 || 3 <= 2) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,true);

    code="if(da_return()==1 || 3 < 2) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,true);

    code="if(3==1 && a<=5) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,true);

    code="if(3==1 && a<<5) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,false);
    
    code="if(3==1 && a == b <= c) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };";
    test(code,false);

    code="a = 1;";
    test(code,true);

    code=" a = 1;";
    test(code,true);

    code="\na = 1;";
    test(code,true);

    code="\ta = 1;";
    test(code,true);

    code="a = 1; ";
    test(code,true);

    code="a = 1;\t";
    test(code,true);

    code=" a = 1;\n";
    test(code,true);

    code=" a = 1; ";
    test(code,true);

    code="\ta=1; ";
    test(code,true);
    
    code="\tfoo(@ROUTINE@); ";
    test(code,true);

    code="a = 1 1;";
    test(code,false);

    code="a a = 1;";
    test(code,false);

    code=" a a = 1; ";
    test(code,false);

    std::cout << "=== finished syntax test ===\n\n";
}

void CodeParserTest::treeOutputTest() {
    CodeParser p;

    std::string code = "a = (1 + d()) + 3; a = 1 +2 + 3; b = 12 + (a*b) + 13 ; if(da_return()==1) { da_enter(@ROUTINE@,@FILE@); };if(a==b){c();if(c==123){e();};};if(a!=b){c();};a = \"abc\";abc = 123; a = f(); bc = de(); cd = f (); ad = cd ; a = 1;b=\"abc\"; f(); f(a);f(a,b);f(123);f(123,\"abc\",\"abc\"); g1(f(),h(a),func());f2(h(123)); a = f(ab,123);f(@ROUTINE@);f(@ROUTINE@,@ROUTINE@);foo(@ROUTINE@.\"-\".@LOOP@);enterName(@ROUTINE@.\"a\");if(a==1) {fprintf(\"a\");}else{fprintf(\"b\");};enterCount(@ROUTINE@,ii);if(da_return()==1 || 3 <= 2 && 3 >=5 ) { da_enter(@ROUTINE@,@FILE@); } else { printf(\"return false\"); };/*blubb*/f();a=a+1;//test\nf();foo(\"\\n1\\n2\\n\\n4\\\\n\\n\\t5\\t\\\\t\\\\t\\n\\t\\t6\\n\\\\\\n\\n\");foo(\" test \".@ROUTINE@.\"\\n\");";
    tree_parse_info<> info = p.getTreeInfoAST(code);

    if (!info.full) {
        return;
    }

    std::cout << "=== test output of ast from parser ===\n";
    std::cout << "root contains " << info.trees.size() << " node\n";

    std::cout << "found " << info.trees[0].children.size() << " statements\n";

    ISnippetFactory* sf = p.getSnippetFactory(CodeParser::DEMO);
    sf->evaluateNode(&(info.trees[0]));
    std::cout << "=== end of test output of ast from parser ===\n\n";
}

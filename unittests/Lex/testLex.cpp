#define __CPP__

#include <gmock/gmock.h>
extern "C" {
#include <tiger-lex.h>
#include <tigerc/Lex/tokens.h>
#include <tigerc/Support/errormsg.h>
}


void lex_setup() {
  yy_scan_string("muffin"); 
  int tok = yylex();
}

using namespace testing; 

TEST(testLex, ForStringReturnsForToken) {
  yy_scan_string("for");
  int output = yylex();
  ASSERT_THAT(output, Eq(FOR));
}

TEST(testLex, ForDoStringReturnsForTokenThenDoToken) {
  yy_scan_string("for do");
  int output = yylex();
  ASSERT_THAT(output, Eq(FOR));
  output = yylex();
  ASSERT_THAT(output, Eq(DO)); 
}

TEST(testLex, OfBreakNilFunctionVarTypeMatchesTokens) {
  yy_scan_string("of break nil function var type");
  int output = yylex();
  ASSERT_THAT(output, Eq(OF));
  output = yylex();
  ASSERT_THAT(output, Eq(BREAK)); 
  output = yylex();
  ASSERT_THAT(output, Eq(NIL)); 
  output = yylex();
  ASSERT_THAT(output, Eq(FUNCTION)); 
  output = yylex();
  ASSERT_THAT(output, Eq(VAR)); 
  output = yylex();
  ASSERT_THAT(output, Eq(TYPE)); 
}

TEST(testLex, AndOrAssignArrayIfThenElseWhileAllWork) {
  yy_scan_string("and or assign array if then else while");
  int output = yylex();
  ASSERT_THAT(output, Eq(AND));
  output = yylex();
  ASSERT_THAT(output, Eq(OR)); 
  output = yylex();
  ASSERT_THAT(output, Eq(ASSIGN)); 
  output = yylex();
  ASSERT_THAT(output, Eq(ARRAY)); 
  output = yylex();
  ASSERT_THAT(output, Eq(IF)); 
  output = yylex();
  ASSERT_THAT(output, Eq(THEN)); 
  output = yylex();
  ASSERT_THAT(output, Eq(ELSE)); 
  output = yylex();
  ASSERT_THAT(output, Eq(WHILE)); 
}

TEST(testLex, NumberReturnsNumberToken) {
  yy_scan_string("41234 4121512 44241 5555");
  for (int i = 0; i < 4; i++) {
    int token = yylex();
    ASSERT_THAT(token, Eq(INT));
  }
}

TEST(testLex, CommaReturnsCommaToken) {
  yy_scan_string(", , , ");
  int token;
  int count = 0;
  while ((token = yylex())) {
    ASSERT_THAT(token, Eq(COMMA));
    count++;
  }
  ASSERT_THAT(count, Eq(3));
}

TEST(testLex, JustSpacesReturnsNoTokensExceptEOF) {
  yy_scan_string("    ");
  int token = yylex();
  ASSERT_THAT(token, Eq(0));
}

TEST(testLex, LoadingAFileWorksUsingTestFileDotTig) {
  string file = String("/Users/lanza/Projects/tigerc/unittests/Lex/testFile.tig");
  FILE *f = fopen(file, "r");
  uintptr_t result = (uintptr_t)f;
  EXPECT_NE(result, 0);
  yyrestart(f);
  int token;
  token = yylex();
  ASSERT_THAT(token, Eq(IF));
  token = yylex();
  ASSERT_THAT(token, Eq(LET));
  token = yylex();
  ASSERT_THAT(token, Eq(COMMA));
}


TEST(testLex, IgnoresCommentsInLineOneOfTest1) {
  string file = String("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  FILE *f = fopen(file, "r");
  yyrestart(f);
  int token = yylex();
  ASSERT_THAT(token, Eq(LET)); 
}

TEST(testLex, IgnoresTabs) {
  yy_scan_string("\tlet \tin");
  int token = yylex();
  ASSERT_THAT(token, Eq(LET));
  token = yylex();
  ASSERT_THAT(token, Eq(IN));
}

TEST(testLex, ParsesEqualSign) {
  yy_scan_string(" = = = ");
  for (int i = 0; i < 3; i++) {
    int token = yylex();
    ASSERT_THAT(token, Eq(EQ));
  }
  int token = yylex();
  ASSERT_THAT(token, Eq(0));
}

TEST(testLex, ParsesAllLowercaseIDsProperly) {
  yy_scan_string("\tmuffin = riley");
  int token = yylex();
  ASSERT_THAT(token, Eq(ID));
  token = yylex();
  ASSERT_THAT(token, Eq(EQ));
  token = yylex();
  ASSERT_THAT(token, Eq(ID));
  token = yylex();
  ASSERT_THAT(token, Eq(0));
}

TEST(testLex, ParsesColonLRBrackets) {
  yy_scan_string(" muffin : [10] :=");
  int token;
  token = yylex();
  ASSERT_THAT(token, Eq(ID));
  token = yylex();
  ASSERT_THAT(token, Eq(COLON));
  token = yylex();
  ASSERT_THAT(token, Eq(LBRACK));
  token = yylex();
  ASSERT_THAT(token, Eq(INT));
  token = yylex();
  ASSERT_THAT(token, Eq(RBRACK));
  token = yylex();
  ASSERT_THAT(token, Eq(ASSIGN));
  token = yylex();
  ASSERT_THAT(token, Eq(0));
}


TEST(testLex, Test1LexesProperly) {
  string file = String("/Users/lanza/Projects/tigerc/unittests/testcases/test1.tig");
  FILE *f = fopen(file, "r");
  yyrestart(f);
  int token; 
  int tokens[] = { LET, TYPE, ID, EQ, ARRAY, OF, ID, VAR, ID, COLON, ID, ASSIGN, ID, LBRACK, INT, RBRACK, OF, INT, IN, ID, END };
  int index = 0;
  while ((token = yylex())) {
    ASSERT_THAT(token, Eq(tokens[index]));
    index++;
  }
}

#define a_tok_equal(x) ASSERT_THAT(token, Eq(x));
#define a_tok_nequal(x) ASSERT_THAT(token, Ne(x));

TEST(testLex, LexLRBrace) {
  yy_scan_string(" { }");
  int token;
  token = yylex();
  a_tok_equal(LBRACE);
  token = yylex();
  a_tok_equal(RBRACE);
}

TEST(testLex, LexPeriod) {
  yy_scan_string(" . muffin.age ");
  int token;
  token = yylex();
  a_tok_equal(DOT);
  token = yylex();
  a_tok_equal(ID);
  token = yylex();
  a_tok_equal(DOT);
  token = yylex();
  a_tok_equal(ID);
}
TEST(testLex, LexSemicolon) {
  yy_scan_string(" ; muffin;age ;");
  int token;
  token = yylex();
  a_tok_equal(SEMICOLON);
  token = yylex();
  a_tok_equal(ID);
  token = yylex();
  a_tok_equal(SEMICOLON);
  token = yylex();
  a_tok_equal(ID);
  token = yylex();
  a_tok_equal(SEMICOLON);
}
TEST(testLex, LexMathematicalOperators) {
  yy_scan_string("+ - * / != < > >= <=");
  int token;
  token = yylex();
  a_tok_equal(PLUS);
  token = yylex();
  a_tok_equal(MINUS);
  token = yylex();
  a_tok_equal(TIMES);
  token = yylex();
  a_tok_equal(DIVIDE);
  token = yylex();
  a_tok_equal(NEQ);
  token = yylex();
  a_tok_equal(LT);
  token = yylex();
  a_tok_equal(GT);
  token = yylex();
  a_tok_equal(GE); 
  token = yylex();
  a_tok_equal(LE); 
}

TEST(testLex, LexString) {
  yy_scan_string(" \" muffin is a dogadoogle \"");
  int token;
  token = yylex();
  a_tok_equal(STRING);
  token = yylex();
  a_tok_equal(0);
}

TEST(testLex, TestFileTwoReturnsNoIllegalTokens) {
    const int SIZE = 1;
    const char *files[SIZE] = {
        "/Users/lanza/Projects/tigerc/unittests/testcases/test2.tig",
    };
    for (int i = 0; i < SIZE; i++) {
        FILE *f = fopen(files[i], "r");
        yyrestart(f);
        int token;
        
        while ((token = yylex())) {
            ASSERT_THAT(token, Ne(666));
        }
    }
}

TEST(testLex, TestFileThreeReturnsNoIllegalTokens) {
  string filename = String("/Users/lanza/Projects/tigerc/unittests/testcases/test3.tig");
  FILE *f = fopen(filename, "r");
  yyrestart(f);
  int token;

  while ((token = yylex())) {
    ASSERT_THAT(token, Ne(666));
  }
}

TEST(testLex, LexParens) {
  yy_scan_string(" ( muffin dog )");
  int token;
  token = yylex();
  a_tok_equal(LPAREN);
  token = yylex();
  a_tok_equal(ID);
  token = yylex();
  a_tok_equal(ID);
  token = yylex();
  a_tok_equal(RPAREN);
  token = yylex();
  a_tok_equal(0);
}


TEST(testLex, TestFileFourReturnsNoIllegalTokens) {
  string filename = String("/Users/lanza/Projects/tigerc/unittests/testcases/test4.tig");
  FILE *f = fopen(filename, "r");
  yyrestart(f);
  int token;

  while ((token = yylex())) {
    ASSERT_THAT(token, Ne(666));
  } 
}


TEST(testLex, TestFileFiveReturnsNoIllegalTokens) {
  string filename = String("/Users/lanza/Projects/tigerc/unittests/testcases/test5.tig");
  FILE *f = fopen(filename, "r");
  yyrestart(f);
  int token;

  while ((token = yylex())) {
    ASSERT_THAT(token, Ne(666));
  } 
}

TEST(testLex, TestFileNineReturnsNoIllegalTokens) {
  string filename = String("/Users/lanza/Projects/tigerc/unittests/testcases/test9.tig");
  FILE *f = fopen(filename, "r");
  yyrestart(f);
  int token;

  while ((token = yylex())) {
    ASSERT_THAT(token, Ne(666));
  } 
}

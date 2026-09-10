#include <string>
#include <vector>

#include "lexer.h"
#include "test_util.h"

using std::string;
using std::vector;

static vector<Token> lex(const string &source) {
  Lexer lexer(source);
  vector<Token> tokens;
  while (lexer.hasMoreTokens())
    tokens.push_back(lexer.nextToken());
  return tokens;
}

static void identifiersAndKeywords() {
  vector<Token> t = lex("int x count_1");
  CHECK(t.size() == 3);
  CHECK(t[0].type == KEYWORD && t[0].lexeme == "int");
  CHECK(t[1].type == ID && t[1].lexeme == "x");
  CHECK(t[2].type == ID && t[2].lexeme == "count_1");
}

static void integersAndChars() {
  vector<Token> t = lex("0 42 'a'");
  CHECK(t.size() == 3);
  CHECK(t[0].type == INTEGER && t[0].lexeme == "0");
  CHECK(t[1].type == INTEGER && t[1].lexeme == "42");
  CHECK(t[2].type == CHAR && t[2].lexeme == "'a'");
}

static void operatorsAndDelimiters() {
  vector<Token> t = lex("+ - * / = == != < <= > >=");
  CHECK(t.size() == 11);
  CHECK(t[0].type == ARITHMETIC_OP);
  CHECK(t[4].type == ASSIGNMENT_OP);
  CHECK(t[5].type == EQ);
  CHECK(t[6].type == NEQ);
  CHECK(t[7].type == LT);
  CHECK(t[8].type == LTE);
  CHECK(t[9].type == GT);
  CHECK(t[10].type == GTE);
}

static void positions() {
  vector<Token> t = lex("int\n  x");
  CHECK(t.size() == 2);
  CHECK(t[0].line == 1 && t[0].column == 1);
  CHECK(t[1].line == 2 && t[1].column == 3);
}

int main() {
  identifiersAndKeywords();
  integersAndChars();
  operatorsAndDelimiters();
  positions();
  return testing::summary("test_lexer");
}

#include "parser.h"

#include <iostream>
#include <string>

using namespace std;

// token handling
void Parser::next() {
  if (lexer.hasMoreTokens()) {
    tok = lexer.nextToken();
    lastLine = tok.line;
  } else {
    tok = {END_OF_FILE, "$", lastLine, 0};
  }
}

void Parser::consume() {
  if (verbose)
    cout << string(depth * 2, ' ') << tokenName(tok.type) << " '"
         << tok.lexeme << "'\n";
  next();
}

void Parser::fail(const string &expected) {
  throw SyntaxError{"Syntax Error at line " + to_string(tok.line) +
                    ", column " + to_string(tok.column) +
                    ": expected " + expected};
}

// lookahead predicates
bool Parser::atKeyword(const string &k) {
  return tok.type == KEYWORD && tok.lexeme == k;
}

bool Parser::atDelim(char c) {
  return tok.type == DELIMITER && tok.lexeme[0] == c;
}

bool Parser::atType() { return atKeyword("int") || atKeyword("char"); }

// matching
void Parser::expectKeyword(const string &k) {
  if (!atKeyword(k))
    fail("'" + k + "'");
  consume();
}

void Parser::expectDelim(char c) {
  if (!atDelim(c))
    fail(string("'") + c + "'");
  consume();
}

void Parser::expectID() {
  if (tok.type != ID)
    fail("identifier");
  consume();
}

// grammar

// Program -> FuncList
void Parser::program() {
  Rule r(*this, "Program");
  while (atType()) // FuncList -> Func FuncList | ^
    func();
  if (tok.type != END_OF_FILE)
    fail("function definition or end of file");
}

// Func -> Type ID ( ParamList ) Block
void Parser::func() {
  Rule r(*this, "Func");
  type();
  expectID();
  expectDelim('(');
  paramList();
  expectDelim(')');
  block();
}

// Type -> int | char
void Parser::type() {
  Rule r(*this, "Type");
  if (!atType())
    fail("type ('int' or 'char')");
  consume();
}

// ParamList -> Param ParamTail | ^
void Parser::paramList() {
  Rule r(*this, "ParamList");
  if (atType()) {
    param();
    while (atDelim(',')) { // ParamTail -> , Param ParamTail | ^
      consume();
      param();
    }
  }
}

// Param -> Type ID
void Parser::param() {
  Rule r(*this, "Param");
  type();
  expectID();
}

// Block -> { StmtList }
void Parser::block() {
  Rule r(*this, "Block");
  expectDelim('{');
  expectDelim('}');
}

Parser::Parser(Lexer &l, bool v) : lexer(l), verbose(v) { next(); }

void Parser::parse() { program(); }

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

bool Parser::atRelop() {
  return tok.type == LT || tok.type == LTE || tok.type == GT ||
         tok.type == GTE || tok.type == EQ || tok.type == NEQ;
}

bool Parser::atAddOp() {
  return tok.type == ARITHMETIC_OP &&
         (tok.lexeme == "+" || tok.lexeme == "-");
}

bool Parser::atMulOp() {
  return tok.type == ARITHMETIC_OP &&
         (tok.lexeme == "*" || tok.lexeme == "/");
}

bool Parser::atExprStart() {
  return tok.type == ID || tok.type == INTEGER || tok.type == CHAR ||
         atDelim('(') || (tok.type == ARITHMETIC_OP && tok.lexeme == "-");
}

bool Parser::atStmtStart() {
  return atType() || tok.type == ID || atDelim('{');
}

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
  while (atStmtStart()) // StmtList -> Stmt StmtList | ^
    stmt();
  expectDelim('}');
}

void Parser::stmt() {
  Rule r(*this, "Stmt");
  if (tok.type == ID)
    idStmt();
  else if (atDelim('{'))
    block();
  else
    fail("statement");
}

// IdStmt -> ID IdStmtRest ;   IdStmtRest -> = Expr | ( ArgList )
void Parser::idStmt() {
  Rule r(*this, "IdStmt");
  expectID();
  if (tok.type == ASSIGNMENT_OP) {
    consume();
    expr();
  } else if (atDelim('(')) {
    consume();
    argList();
    expectDelim(')');
  } else {
    fail("'=' or '('");
  }
  expectDelim(';');
}

// Expr -> Arith RelTail   RelTail -> relop Arith | ^
void Parser::expr() {
  Rule r(*this, "Expr");
  arith();
  if (atRelop()) {
    consume();
    arith();
  }
}

// Arith -> Term ArithTail   ArithTail -> addop Term ArithTail | ^
void Parser::arith() {
  Rule r(*this, "Arith");
  term();
  while (atAddOp()) {
    consume();
    term();
  }
}

// Term -> Factor TermTail   TermTail -> mulop Factor TermTail | ^
void Parser::term() {
  Rule r(*this, "Term");
  factor();
  while (atMulOp()) {
    consume();
    factor();
  }
}

// Factor -> ID FactorRest | INTEGER | CHAR | ( Expr ) | - Factor
void Parser::factor() {
  Rule r(*this, "Factor");
  if (tok.type == ID) {
    consume();
    if (atDelim('(')) { // FactorRest -> ( ArgList ) | ^
      consume();
      argList();
      expectDelim(')');
    }
  } else if (tok.type == INTEGER || tok.type == CHAR) {
    consume();
  } else if (atDelim('(')) {
    consume();
    expr();
    expectDelim(')');
  } else if (tok.type == ARITHMETIC_OP && tok.lexeme == "-") {
    consume();
    factor();
  } else {
    fail("expression");
  }
}

// ArgList -> Expr ArgTail | ^   ArgTail -> , Expr ArgTail | ^
void Parser::argList() {
  Rule r(*this, "ArgList");
  if (atExprStart()) {
    expr();
    while (atDelim(',')) {
      consume();
      expr();
    }
  }
}

Parser::Parser(Lexer &l, bool v) : lexer(l), verbose(v) { next(); }

void Parser::parse() { program(); }

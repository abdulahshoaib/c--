#pragma once

#include <iostream>
#include <string>

#include "lexer.h"
#include "token.h"

struct SyntaxError {
  std::string message;
};

class Parser {
private:
  Lexer &lexer;
  Token tok;
  int lastLine = 1;
  bool verbose;
  int depth = 0;

  // Prints the parse tree as rules are entered (RAII handles indentation)
  struct Rule {
    Parser &p;
    Rule(Parser &parser, const std::string &name) : p(parser) {
      if (p.verbose)
        std::cout << std::string(p.depth * 2, ' ') << name << "\n";
      p.depth++;
    }
    ~Rule() { p.depth--; }
  };

  // token handling
  void next();
  void consume();
  [[noreturn]] void fail(const std::string &expected);

  // lookahead predicates
  bool atKeyword(const std::string &k);
  bool atDelim(char c);
  bool atType();
  bool atRelop();
  bool atAddOp();
  bool atMulOp();
  bool atExprStart();
  bool atStmtStart();

  // matching
  void expectKeyword(const std::string &k);
  void expectDelim(char c);
  void expectID();

  // grammar
  void program();
  void func();
  void type();
  void paramList();
  void param();
  void block();
  void stmt();
  void varDecl();
  void initOpt();
  void idStmt();
  void ifStmt();
  void whileStmt();
  void inStmt();
  void outStmt();
  void returnStmt();
  void expr();
  void arith();
  void term();
  void factor();
  void argList();

public:
  Parser(Lexer &l, bool v = false);

  void parse();
};

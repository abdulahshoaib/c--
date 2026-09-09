#pragma once

#include <string>
#include <unordered_set>

#include "token.h"

class Lexer {
private:
  std::string source;
  int position;
  int line;
  int column;

  std::unordered_set<std::string> keywords = {"int", "char", "if",    "else",
                                              "in",  "out",  "while", "return"};

  char currentChar();
  void advance();
  void skipWhitespace();

  Token makeToken(TokenType type, std::string lexeme, int startLine,
                  int startColumn);

  Token scanID();
  Token scanInteger();
  Token scanChar();

public:
  Lexer(std::string input);

  bool hasMoreTokens();
  Token nextToken();
};

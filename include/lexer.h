#pragma once

#include <string>

#include "token.h"

class Lexer {
private:
  std::string source;
  int position;
  int line;
  int column;

  char currentChar();
  void advance();
  void skipWhitespace();

  Token makeToken(TokenType type, std::string lexeme, int startLine,
                  int startColumn);

  Token scanID();
  Token scanInteger();

public:
  Lexer(std::string input);

  bool hasMoreTokens();
  Token nextToken();
};

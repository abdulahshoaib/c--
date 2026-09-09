#pragma once

#include <string>

enum TokenType {
  ID,
  INTEGER,
  CHAR,
  ERROR,

  ARITHMETIC_OP,

  ASSIGNMENT_OP,

  DELIMITER,

  KEYWORD,

  END_OF_FILE
};

struct Token {
  TokenType type;
  std::string lexeme;
  int line;
  int column;
};

std::string tokenName(TokenType type);

#include "lexer.h"

#include <cctype>

using namespace std;

char Lexer::currentChar() {
  if (position >= (int)source.length())
    return '\0';

  return source[position];
}

void Lexer::advance() {
  if (position >= (int)source.length())
    return;

  if (source[position] == '\n') {
    line++;
    column = 1;
  } else {
    column++;
  }

  position++;
}

void Lexer::skipWhitespace() {
  while (isspace(currentChar())) {
    advance();
  }
}

Token Lexer::makeToken(TokenType type, string lexeme, int startLine,
                       int startColumn) {
  return {type, lexeme, startLine, startColumn};
}

// ID
Token Lexer::scanID() {
  int startLine = line;
  int startColumn = column;

  string lexeme;

  while (isalpha(currentChar()) || isdigit(currentChar()) ||
         currentChar() == '_') {
    lexeme += currentChar();
    advance();
  }

  return makeToken(ID, lexeme, startLine, startColumn);
}

// INTEGER
Token Lexer::scanInteger() {
  int startLine = line;
  int startColumn = column;

  string lexeme;

  while (isdigit(currentChar())) {
    lexeme += currentChar();
    advance();
  }

  return makeToken(INTEGER, lexeme, startLine, startColumn);
}

Lexer::Lexer(string input) {
  source = input;
  position = 0;
  line = 1;
  column = 1;
}

bool Lexer::hasMoreTokens() {
  skipWhitespace();

  return position < (int)source.length();
}

Token Lexer::nextToken() {
  skipWhitespace();

  int startLine = line;
  int startColumn = column;

  char c = currentChar();

  // ID
  if (isalpha(c) || c == '_') {
    return scanID();
  }

  // INTEGER
  if (isdigit(c)) {
    return scanInteger();
  }

  // ARITHMETIC OPERATORS
  if (c == '+' || c == '-' || c == '*' || c == '/') {
    string lexeme(1, c);

    advance();

    return makeToken(ARITHMETIC_OP, lexeme, startLine, startColumn);
  }

  // =
  if (c == '=') {
    advance();

    return makeToken(ASSIGNMENT_OP, "=", startLine, startColumn);
  }

  // DELIMITERS
  if (c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',') {
    string lexeme(1, c);

    advance();

    return makeToken(DELIMITER, lexeme, startLine, startColumn);
  }

  // Unknown character
  advance();

  return makeToken(ERROR, string(1, c), startLine, startColumn);
}

#include "token.h"

using namespace std;

string tokenName(TokenType type) {
  switch (type) {
  case ID:
    return "ID";
  case INTEGER:
    return "INTEGER";
  case ERROR:
    return "ERROR";
  case ARITHMETIC_OP:
    return "ARITHMETIC_OP";
  case ASSIGNMENT_OP:
    return "ASSIGNMENT_OP";
  case DELIMITER:
    return "DELIMITER";
  case END_OF_FILE:
    return "EOF";
  }

  return "";
}

#include "token.h"

using namespace std;

string tokenName(TokenType type) {
  switch (type) {
  case ID:
    return "ID";
  case INTEGER:
    return "INTEGER";
  case CHAR:
    return "CHAR";
  case ERROR:
    return "ERROR";
  case ARITHMETIC_OP:
    return "ARITHMETIC_OP";
  case LT:
    return "RELATIONAL_OP_LT";
  case LTE:
    return "RELATIONAL_OP_LTE";
  case GT:
    return "RELATIONAL_OP_GT";
  case GTE:
    return "RELATIONAL_OP_GTE";
  case EQ:
    return "RELATIONAL_OP_EQ";
  case NEQ:
    return "RELATIONAL_OP_NEQ";
  case ASSIGNMENT_OP:
    return "ASSIGNMENT_OP";
  case DELIMITER:
    return "DELIMITER";
  case KEYWORD:
    return "KEYWORD";
  case END_OF_FILE:
    return "EOF";
  }

  return "";
}

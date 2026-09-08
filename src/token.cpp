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
  case END_OF_FILE:
    return "EOF";
  }

  return "";
}

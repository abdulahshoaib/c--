#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "lexer.h"
#include "token.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: lexer <source-file>\n";
    return 1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cout << "Error: Could not open file.\n";
    return 1;
  }

  string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  file.close();

  Lexer lexer(source);

  while (lexer.hasMoreTokens()) {
    Token tok = lexer.nextToken();
    cout << tokenName(tok.type) << " '" << tok.lexeme << "'\n";
  }

  return 0;
}

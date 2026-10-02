#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: parser <source-file> [-v]\n";
    return 1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cout << "Error: Could not open file.\n";
    return 1;
  }

  string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  file.close();

  bool verbose = (argc > 2 && string(argv[2]) == "-v");

  Lexer lexer(source);
  Parser parser(lexer, verbose);

  try {
    parser.parse();
  } catch (const SyntaxError &e) {
    cout << e.message << "\n";
    return 1;
  }

  cout << "Parsing completed successfully.\n";
  return 0;
}

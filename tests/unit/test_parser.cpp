#include <string>

#include "lexer.h"
#include "parser.h"
#include "test_util.h"

using std::string;

static bool parses(const string &source) {
  Lexer lexer(source);
  Parser parser(lexer);
  try {
    parser.parse();
  } catch (const SyntaxError &) {
    return false;
  }
  return true;
}

static void accepts() {
  CHECK(parses("int f() { return 0; }"));
  CHECK(parses("int add(int a, int b) { return a + b; }"));
  CHECK(parses("char first(char c) { return c; }"));
  CHECK(parses("int main() { int a, b = 2; a = b + 1; out a; }"));
  CHECK(parses("int f() { if (a < b) out a; else out b; }"));
  CHECK(parses("int f() { while (a < 10) a = a + 1; }"));
  CHECK(parses("int f() { in x; out x * 2 - 1; }"));
  CHECK(parses("int f() { g(1, add(2, 3)); }"));
}

static void rejects() {
  CHECK(!parses("int"));
  CHECK(!parses("int f() { x = ; }"));
  CHECK(!parses("int f() { if (a < b out a; }"));
  CHECK(!parses("int f() { return 0 }"));
  CHECK(!parses("int f() { x = 1;"));
  CHECK(!parses("f() {}"));
}

int main() {
  accepts();
  rejects();
  return testing::summary("test_parser");
}

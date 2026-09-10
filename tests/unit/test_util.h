#pragma once

#include <iostream>
#include <string>

// Tiny dependency-free test harness: call CHECK() from test functions and
// finish with testing::summary(name) as the program's return value.
namespace testing {

inline int g_checks = 0;
inline int g_failures = 0;

inline void record(bool ok, const std::string &expr, const char *file,
                   int line) {
  ++g_checks;
  if (!ok) {
    ++g_failures;
    std::cerr << file << ":" << line << ": check failed: " << expr << "\n";
  }
}

inline int summary(const std::string &name) {
  std::cout << name << ": " << (g_checks - g_failures) << "/" << g_checks
            << " checks passed\n";
  return g_failures == 0 ? 0 : 1;
}

} // namespace testing

#define CHECK(cond) ::testing::record((cond), #cond, __FILE__, __LINE__)

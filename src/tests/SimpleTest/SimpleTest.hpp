#ifndef SIMPLETEST_HPP
#define SIMPLETEST_HPP

#include <sstream>
#include <string>
#include <vector>

#define TEST(lparam, op, rparam)                            \
  if (lparam == rparam)                                     \
  {                                                         \
    std::stringstream ss;                                   \
    ss << "\e[32m[PASS]\e[0m "                              \
       << __PRETTY_FUNCTION__ << ":" << __LINE__            \
       << " " << #lparam << " " << #op << " " << #rparam    \
       << " -> " << lparam << " " << #op << " " << #rparam; \
    std::cout << ss.str() << std::endl;                     \
    SimpleTest::addPassFunc(ss.str());                      \
    SimpleTest::passCount++;                                \
  }                                                         \
  else                                                      \
  {                                                         \
    std::stringstream ss;                                   \
    ss << "\e[31m[FAIL]\e[0m "                              \
       << __PRETTY_FUNCTION__ << ":" << __LINE__            \
       << " " << #lparam << " " << #op << " " << #rparam    \
       << " -> " << lparam << " " << #op << " " << #rparam; \
    std::cout << ss.str() << std::endl;                     \
    SimpleTest::addFailFunc(ss.str());                      \
    SimpleTest::failCount++;                                \
  }

#define START_TEST()                                       \
  std::cout << "\n\e[35m" << __PRETTY_FUNCTION__           \
            << "\n=======================================" \
            << "=========================================" \
            << "\e[0m" << std::endl;

class SimpleTest
{
  SimpleTest();
  SimpleTest(const SimpleTest&) = delete;
  ~SimpleTest() = delete;

public:
  static unsigned int passCount;
  static unsigned int failCount;

  static void addFailFunc(const std::string& func);
  static void addPassFunc(const std::string& func);
  static std::string getReport();

  static bool isSuccess();

private:
  static std::vector<std::string> failFuncs;
  static std::vector<std::string> passFuncs;
};
#endif

#ifndef SIMPLETEST_HPP
#define SIMPLETEST_HPP

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define TEST(lparam, op, rparam)                           \
  if (lparam op rparam)                                    \
  {                                                        \
    std::stringstream ss;                                  \
    ss << "\e[32m[PASS]\e[0m "                             \
       << __PRETTY_FUNCTION__ << ":" << __LINE__           \
       << " " << #lparam << " " << #op << " " << #rparam   \
       << " -> " << lparam << " " << #op << " " << rparam; \
    std::cout << ss.str() << std::endl;                    \
    SimpleTest::addPassFunc(ss.str());                     \
  }                                                        \
  else                                                     \
  {                                                        \
    std::stringstream ss;                                  \
    ss << "\e[31m[FAIL]\e[0m "                             \
       << __PRETTY_FUNCTION__ << ":" << __LINE__           \
       << " " << #lparam << " " << #op << " " << #rparam   \
       << " -> " << lparam << " " << #op << " " << rparam; \
    std::cout << ss.str() << std::endl;                    \
    SimpleTest::addFailFunc(ss.str());                     \
  }

#define ADD_TEST(func)                                                                      \
  funcs.push_back(std::make_pair<std::string, std::function<bool(void)>>(#func,             \
                                                                         [this]() -> bool { \
                                                                           return func();   \
                                                                         }));

class SimpleTest
{
public:
  SimpleTest();
  SimpleTest(const SimpleTest&) = delete;
  ~SimpleTest();

  static void addFailFunc(const std::string& func);
  static void addPassFunc(const std::string& func);
  static std::string getReport();

  static bool isSuccess();

  bool runAllTests();

protected:
  std::vector<std::pair<std::string, std::function<bool(void)>>> funcs;

private:
  static std::vector<std::string> failFuncs;
  static std::vector<std::string> passFuncs;
};
#endif

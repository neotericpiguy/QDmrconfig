#include "SimpleTest.hpp"

std::vector<std::string> SimpleTest::failFuncs = {};
std::vector<std::string> SimpleTest::passFuncs = {};

SimpleTest::SimpleTest() :
    funcs()
{
}

SimpleTest::~SimpleTest()
{
}

void SimpleTest::addFailFunc(const std::string& func)
{
  failFuncs.push_back(func);
}

void SimpleTest::addPassFunc(const std::string& func)
{
  passFuncs.push_back(func);
}

std::string SimpleTest::getReport()
{
  std::stringstream ss;
  ss << "\e[32m" << passFuncs.size() << " PASS out of " << passFuncs.size() + failFuncs.size() << "\e[0m\n";
  for (const auto& func : failFuncs)
    ss << func << "\n";

  return ss.str();
}

bool SimpleTest::isSuccess()
{
  return failFuncs.size() == 0;
}

bool SimpleTest::runAllTests()
{
  for (const auto& funcPair : funcs)
  {
    std::cout << "\n\e[35m" << funcPair.first
              << "\n======================================="
              << "========================================="
              << "\e[0m" << std::endl;
    funcPair.second();
  }
  return true;
}

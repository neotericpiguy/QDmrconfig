#include "SimpleTest.hpp"

unsigned int SimpleTest::passCount = 0;
unsigned int SimpleTest::failCount = 0;

std::vector<std::string> SimpleTest::failFuncs = {};
std::vector<std::string> SimpleTest::passFuncs = {};

SimpleTest::SimpleTest()
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
  ss << "\e[32m" << passCount << " PASS out of " << passFuncs.size() + failFuncs.size() << "\e[0m\n";
  for (const auto& func : failFuncs)
    ss << func << "\n";

  return ss.str();
}

bool SimpleTest::isSuccess()
{
  return failFuncs.size() == 0;
}
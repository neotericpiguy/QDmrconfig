#include <iostream>

#include "BSONDocTests.hpp"
#include "SimpleTest.hpp"

int main()
{
  BSONDocTests bsonDocTests;
  bsonDocTests.runAllTests();

  std::cout << SimpleTest::getReport();
  return !SimpleTest::isSuccess();
}

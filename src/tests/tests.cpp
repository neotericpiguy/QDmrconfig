#include <iostream>

#include "BSONDocTests.hpp"
#include "NetworkApiTests.hpp"
#include "SimpleTest.hpp"

int main()
{
  BSONDocTests bsonDocTests;
  bsonDocTests.runAllTests();

  NetworkApiTests networkApiTests;
  networkApiTests.runAllTests();

  std::cout << SimpleTest::getReport();
  return !SimpleTest::isSuccess();
}

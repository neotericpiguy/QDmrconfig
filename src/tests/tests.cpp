#include <iostream>

#include "BSONDocTests.hpp"
#include "NetworkApiTests.hpp"
#include "SimpleTest.hpp"

int main(int argc, char** argv)
{
  BSONDocTests bsonDocTests;
  bsonDocTests.runAllTests();

  //NetworkApiTests networkApiTests(argc, argv);
  //  networkApiTests.runAllTests();
  QApplication app(argc, argv);
  NetworkApi networkApi;
  networkApi.simple();

  app.exec();

  std::cout << SimpleTest::getReport();
  return !SimpleTest::isSuccess();
}

#include <iostream>

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#pragma GCC diagnostic pop

#include "BSONDocTests.hpp"
#include "NetworkApiTests.hpp"
#include "SimpleTest.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("QDmrconfig-unit-tests");
  QCoreApplication::setApplicationVersion(VERSION);

  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOptions({
      {{"n", "net-tests"}, "Only net tests."},
      {{"b", "bson-tests"}, "only bson parsing test."},
  });
  parser.process(app);

  bool allTests = (argc == 1);

  if (parser.isSet("bson-tests") || allTests)
  {
    BSONDocTests bsonDocTests;
    bsonDocTests.runAllTests();
  }

  if (parser.isSet("net-tests") || allTests)
  {
    NetworkApi networkApi;
    networkApi.simple();
    app.exec();
  }

  std::cout << SimpleTest::getReport();
  return !SimpleTest::isSuccess();
}

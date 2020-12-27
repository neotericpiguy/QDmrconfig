#include <iostream>

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#pragma GCC diagnostic pop

#include "BSONDocTests.hpp"
#include "NetworkApiTests.hpp"
#include "SimpleTest.hpp"
#include "WidgetTests.cpp"

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
      {{"b", "bson-tests"}, "Only bson parsing tests."},
      {{"w", "widget-tests"}, "Only widget tests."},
  });
  parser.process(app);

  bool allTests = (argc == 1);

  if (parser.isSet("bson-tests") || allTests)
  {
    BSONDocTests bsonDocTests;
    bsonDocTests.runAllTests();
  }

  if (parser.isSet("net-tests"))
  {
    NetworkApiTests networkApiTests;
    networkApiTests.fccCallsignSearchTest();
    networkApiTests.repeaterSearchTest();
    app.exec();
  }

  if (parser.isSet("widget-tests"))
  {
    WidgetTests widgetTests;
    widgetTests.show();
    app.exec();
  }

  std::cout << SimpleTest::getReport();
  return !SimpleTest::isSuccess();
}

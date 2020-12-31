#include <iostream>

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#pragma GCC diagnostic pop

#include "BSONDocTests.hpp"
#include "ChirpCsvTests.cpp"
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
      {{"b", "bsonDocTests"}, "Only bson parsing tests."},
      {{"c", "chirpCsvTest"}, "Only Test Chirp parsing tests."},
      {{"n", "netTests"}, "Only net tests."},
      {{"w", "widget-tests"}, "Only widget tests."},
  });
  parser.process(app);

  bool allTests = (argc == 1);

  if (parser.isSet("bsonDocTests") || allTests)
  {
    BSONDocTests bsonDocTests;
    bsonDocTests.runAllTests();
  }

  if (parser.isSet("chirpCsvTest") || allTests)
  {
    ChirpCsvTests chirpCsvTests;
    chirpCsvTests.runAllTests();
  }

  if (parser.isSet("chirpCsvTest") || allTests)
  {
    ChirpCsvTests chirpCsvTests;
    chirpCsvTests.runAllTests();
  }

  if (parser.isSet("netTests"))
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

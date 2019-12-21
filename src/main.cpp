#include <stdlib.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QtDebug>
#include <QtWidgets/QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
  char setting[] = "QT_MESSAGE_PATTERN=[%{type}] %{appname} (%{file}:%{line}) - %{message}";
  putenv(setting);

  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("QDmrconfig");

  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file", "The file to open.");
  parser.process(app);

  MainWindow mainWin;
  if (!parser.positionalArguments().isEmpty())
    mainWin.loadFile(parser.positionalArguments().first());

  qDebug() << "Starting";
  mainWin.show();

  return app.exec();
}

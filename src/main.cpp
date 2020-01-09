#include <stdlib.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QtDebug>
#include <QtWidgets/QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
  char setting[] = "QT_MESSAGE_PATTERN=%{file}:%{line} - %{message}";
  putenv(setting);

  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("QDmrconfig");
  QCoreApplication::setApplicationVersion(VERSION);

  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file", "The file to open.");
  parser.addOptions({
      // A boolean option with multiple names (-f, --force)
      {{"d", "debug"},
       QCoreApplication::translate("main", "Debug")},
  });
  parser.process(app);

  MainWindow mainWin;

  if (parser.isSet("debug"))
  {
    mainWin.setDebug(true);
    qDebug() << "Starting";
  }

  if (!parser.positionalArguments().isEmpty())
    mainWin.loadFile(parser.positionalArguments().first());

  mainWin.show();

  return app.exec();
}

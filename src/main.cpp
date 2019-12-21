#include <stdlib.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QtDebug>
#include <QtWidgets/QApplication>

#include "MainWindow.hpp"

//int main()
//{
////  std::cout << "Starting" << std::endl;
////  // Dump device to image file.
////  radio_connect();
////  radio_download();
////  radio_print_version(stdout);
////  radio_disconnect();
////  radio_save_image("device.img");
////
////  // Print configuration to file.
////  const char* filename = "device.conf";
////  printf("Print configuration to file '%s'.\n", filename);
////  FILE* conf = fopen(filename, "w");
////  if (!conf)
////  {
////    perror(filename);
////    exit(-1);
////  }
////  radio_print_config(conf, 1);
////  fclose(conf);
////  return 0;
//}
//

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

#include <stdlib.h>

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#pragma GCC diagnostic pop

#include <iostream>

#include "MainWindow.hpp"
#include "StringThings.hpp"

extern "C" const char version[] = VERSION;
const char* copyright;

extern char* optarg;
extern int optind;

extern "C" void radio_connect(int);
extern "C" void radio_download(void);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_disconnect(void);
extern "C" void radio_save_image(const char* filename);
extern "C" void radio_print_config(FILE* out, int verbose);
extern "C" void radio_parse_config(const char* filename);
extern "C" void radio_verify_config();
extern "C" void radio_upload(int cont_flag);

void radioUpload(const std::string& file)
{
  radio_connect(-1);
  radio_download();
  radio_print_version(stdout);
  radio_save_image("backup.img");
  radio_parse_config(file.c_str());
  radio_verify_config();
  radio_upload(1);
  radio_disconnect();
}

void radioDownload(const std::string& file)
{
  std::string imageFilename = file;
  StringThings::replace(imageFilename, ".conf", ".img");
  radio_connect(-1);
  radio_download();
  radio_print_version(stdout);
  radio_disconnect();
  radio_save_image(imageFilename.c_str());

  std::cout << "Print configuration to file " << file << std::endl;

  FILE* conf = fopen(file.c_str(), "w");
  if (!conf)
  {
    perror(file.c_str());
    exit(-1);
  }
  radio_print_config(conf, 1);
  fclose(conf);
}

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

  auto uploadFunc = [](const std::string& file) -> void {
    radioUpload(file);
  };

  auto downloadFunc = [](const std::string& file) -> void {
    radioDownload(file);
  };

  MainWindow mainWin(uploadFunc, downloadFunc);

  if (parser.isSet("debug"))
  {
    mainWin.setDebug(true);
    qDebug() << "Starting";
  }

  if (!parser.positionalArguments().isEmpty())
    mainWin.loadFile(parser.positionalArguments().first().toStdString());

  mainWin.show();

  return app.exec();
}

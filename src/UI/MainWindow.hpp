#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#pragma GCC diagnostic pop

#include "RepeaterBook.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile);
  virtual ~MainWindow();

  void loadFile(const std::string& fileName);
  void closeEvent(QCloseEvent* event) override;
  void setDebug(bool state);

  void repeaterBookSearch();
  void processRepeaterBookSearchResults();

private slots:
  void callsignSearchReady(QNetworkReply* reply);
  void repeaterBookSlotReadyRead(QNetworkReply* reply);

private:
  std::string _fccSearchString;
  std::string _repeaterBookSearchString;

  QNetworkAccessManager* _networkManager;
  QNetworkAccessManager* _repeaterBookNetworkManager;
  QTabWidget* _tabWidget;

  std::function<void(const std::string&)> radioUploadFile;
  std::function<void(const std::string&)> radioDownloadFile;

  bool _debug;

  bool _multiRepeaterBookSearch;
  RepeaterBook _multiRepeaterBookSearchResults;
};

#endif

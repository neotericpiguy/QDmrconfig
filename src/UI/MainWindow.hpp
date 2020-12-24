#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#pragma GCC diagnostic ignored "-Weffc++"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlSimpleReader>
#include <QtWidgets>
#pragma GCC diagnostic pop

#include <memory>

#include "BSONDoc.hpp"
#include "BSONDocWidget.hpp"
#include "ConfFileWidget.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  virtual ~MainWindow();

  void loadFile(const QString& fileName);
  void closeEvent(QCloseEvent* event) override;
  void setDebug(bool state);

private slots:
  void callsignSearchReady(QNetworkReply* reply);
  void repeaterBookSlotReadyRead(QNetworkReply* reply);

private:
  std::vector<Mongo::BSONDoc> _bsonResults;
  std::string _fccSearchString;
  std::string _repeaterBookSearchString;
  ConfFileWidget* _confFileWidget;
  BSONDocWidget* _bsonDocWidget;
  QNetworkAccessManager* _networkManager;
  QNetworkAccessManager* _repeaterBookNetworkManager;
};

#endif

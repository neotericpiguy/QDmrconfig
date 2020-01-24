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
  void slotReadyRead(QNetworkReply* reply);

private:
  ConfFileWidget* _confFileWidget;
  QNetworkAccessManager* _networkManager;
};

#endif

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
  QNetworkAccessManager* _networkAccessManager;
};

class Parser : public QXmlDefaultHandler
{
public:
  Parser()
  {
  }

  bool startDocument()
  {
    qDebug() << "Searching document for tags";
    return true;
  }

  bool startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& /*att*/);
  bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
};

#endif

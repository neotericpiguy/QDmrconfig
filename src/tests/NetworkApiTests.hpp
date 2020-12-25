#ifndef NETWORKAPITESTS_HPP
#define NETWORKAPITESTS_HPP

#include <QApplication>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "BSONDoc.hpp"
#include "SimpleTest.hpp"

class NetworkApi : public QMainWindow
{
  Q_OBJECT

public:
  NetworkApi();
  NetworkApi(const NetworkApi&) = delete;
  ~NetworkApi();

  bool simple();
  bool attemptClose();

private slots:
  void callsignSearchReady(QNetworkReply* reply);

private:
  QNetworkAccessManager* _networkManager;
  int callsignSearchAttempts;
};

#endif

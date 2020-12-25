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

  bool fccCallsignSearchTest();
  bool repeaterSearchTest();
  bool attemptClose();

private slots:
  void fccCallsignSearchReady(QNetworkReply* reply);
  void repeaterSearchReady(QNetworkReply* reply);

private:
  QNetworkAccessManager* _fccCallsignSearchNetworkManager;
  QNetworkAccessManager* _repeaterSearchNetworkManager;

  std::vector<int> attempts;
  int& callsignSearchAttempts;
  int& repeaterSearchAttempts;
};

#endif

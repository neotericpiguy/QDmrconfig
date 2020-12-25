#include "NetworkApiTests.hpp"

NetworkApi::NetworkApi() :
    QMainWindow(),
    _networkManager(new QNetworkAccessManager(this))
{
  connect(_networkManager, &QNetworkAccessManager::finished,
          this, &NetworkApi::callsignSearchReady);
}

NetworkApi::~NetworkApi()
{
}

bool NetworkApi::simple()
{
  QString url("https://data.fcc.gov/api/license-view/basicSearch/getLicenses?format=json&searchValue=KJ7LEY");

  QNetworkRequest request;
  request.setUrl(QUrl(url));

  _networkManager->get(request);
  std::cout << "Searching fcc callsign: KJ7LEY" << std::endl;
  return true;
}

void NetworkApi::callsignSearchReady(QNetworkReply* reply)
{
  QString replyStr(reply->readAll());
  reply->deleteLater();

  static int attempts = 3;
  TEST(replyStr.toStdString().empty(), ==, false);
  if (replyStr.toStdString().empty() && attempts-- >= 0)
  {
    simple();
    return;
  }

  Mongo::BSONDoc results(replyStr.toStdString());
  if (results.has("Errors") && attempts-- >= 0)
  {
    simple();
    return;
  }

  std::cout << results.toString() << std::endl;

  TEST(results.has("Licenses.License"), ==, true);
  close();
}


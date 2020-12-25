#include "NetworkApiTests.hpp"

NetworkApi::NetworkApi() :
    QMainWindow(),
    _networkManager(new QNetworkAccessManager(this)),
    callsignSearchAttempts(3)
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

  if (replyStr.toStdString().empty() && --callsignSearchAttempts)
  {
    std::cout << " Empty response" << std::endl;
    simple();
    return;
  }

  // Failed after 3 attempts
  if (callsignSearchAttempts <= 0)
  {
    TEST(callsignSearchAttempts, >, 0);
    attemptClose();
    return;
  }

  TEST(replyStr.toStdString().empty(), ==, false);
  Mongo::BSONDoc results(replyStr.toStdString());
  if (results.has("Errors") && --callsignSearchAttempts)
  {
    simple();
    return;
  }

  if (callsignSearchAttempts <= 0)
  {
    TEST(callsignSearchAttempts, >, 0)
    attemptClose();
    return;
  }

  std::cout << results.toString() << std::endl;

  TEST(results.has("Licenses.License"), ==, true);

  // Important so that we can keep track of async funcs that finished
  callsignSearchAttempts = 0;
}

bool NetworkApi::attemptClose()
{
  std::cout << "attempt Close" << callsignSearchAttempts << std::endl;
  if (callsignSearchAttempts == 0)
  {
    close();
  }
  return true;
}

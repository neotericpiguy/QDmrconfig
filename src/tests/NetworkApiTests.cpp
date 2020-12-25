#include "NetworkApiTests.hpp"

NetworkApi::NetworkApi() :
    QMainWindow(),
    _fccCallsignSearchNetworkManager(new QNetworkAccessManager(this)),
    _repeaterSearchNetworkManager(new QNetworkAccessManager(this)),
    attempts({3, 3}),
    callsignSearchAttempts(attempts[0]),
    repeaterSearchAttempts(attempts[1])
{
  connect(_fccCallsignSearchNetworkManager, &QNetworkAccessManager::finished,
          this, &NetworkApi::fccCallsignSearchReady);

  connect(_repeaterSearchNetworkManager, &QNetworkAccessManager::finished,
          this, &NetworkApi::repeaterSearchReady);
}

NetworkApi::~NetworkApi()
{
}

bool NetworkApi::fccCallsignSearchTest()
{
  QString url("https://data.fcc.gov/api/license-view/basicSearch/getLicenses?format=json&searchValue=KJ7LEY");

  QNetworkRequest request;
  request.setUrl(QUrl(url));

  _fccCallsignSearchNetworkManager->get(request);
  std::cout << "Searching fcc callsign: KJ7LEY" << std::endl;
  return true;
}

bool NetworkApi::repeaterSearchTest()
{
  QString url("https://www.repeaterbook.com/api/export.php?city=vail");

  QNetworkRequest request;
  request.setUrl(QUrl(url));

  _repeaterSearchNetworkManager->get(request);
  std::cout << "Repeater search in vail" << std::endl;
  return true;
}

void NetworkApi::fccCallsignSearchReady(QNetworkReply* reply)
{
  QString replyStr(reply->readAll());
  reply->deleteLater();

  if (replyStr.toStdString().empty() && --callsignSearchAttempts)
  {
    std::cout << " Empty response" << std::endl;
    fccCallsignSearchTest();
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
    std::cout << results.toString() << std::endl;
    TEST(results.has("Errors"), ==, true);
    TEST(results.has("Errors.Err"), ==, true);
    TEST(results.get<std::string>("Errors.Err.0.code"), !=, "");
    TEST(results.get<std::string>("Errors.Err.0.msg"), !=, "");
    fccCallsignSearchTest();
    return;
  }

  if (callsignSearchAttempts <= 0)
  {
    TEST(callsignSearchAttempts, >, 0)
    attemptClose();
    return;
  }

  std::cout << results.toString() << std::endl;

  TEST(results.get<std::string>("status"), ==, "OK");
  TEST(results.get<std::string>("Licenses.page"), ==, "1");
  TEST(results.get<std::string>("Licenses.rowPerPage"), ==, "100");
  TEST(results.get<std::string>("Licenses.totalRows"), ==, "1");
  TEST(results.has("Licenses.lastUpdate"), ==, true);
  TEST(results.has("Licenses.License"), ==, true);
  TEST(results.get<std::string>("Licenses.License.0.licName"), ==, "Annua, Jonathan Lee O");
  TEST(results.get<std::string>("Licenses.License.0.frn"), ==, "0029046729");
  TEST(results.get<std::string>("Licenses.License.0.callsign"), ==, "KJ7LEY");
  TEST(results.get<std::string>("Licenses.License.0.categoryDesc"), ==, "Personal Use");
  TEST(results.get<std::string>("Licenses.License.0.serviceDesc"), ==, "Amateur");
  TEST(results.get<std::string>("Licenses.License.0.statusDesc"), ==, "Active");
  TEST(results.get<std::string>("Licenses.License.0.expiredDate"), ==, "12/12/2029");
  TEST(results.get<std::string>("Licenses.License.0.licenseID"), ==, "4232049");

  // Important so that we can keep track of async funcs that finished
  callsignSearchAttempts = 0;
  attemptClose();
}

void NetworkApi::repeaterSearchReady(QNetworkReply* reply)
{
  QString replyStr(reply->readAll());
  reply->deleteLater();

  if (replyStr.toStdString().empty() && --repeaterSearchAttempts)
  {
    std::cout << " Empty response" << std::endl;
    repeaterSearchTest();
    return;
  }

  // Failed after 3 attempts
  if (repeaterSearchAttempts <= 0)
  {
    TEST(repeaterSearchAttempts, >, 0);
    attemptClose();
    return;
  }

  Mongo::BSONDoc results(replyStr.toStdString());

  TEST(results.get<int32_t>("count"), >, 0);
  unsigned int count = results.get<int32_t>("count");
  TEST(results.has("results"), ==, true);

  auto docs = results.get<std::vector<Mongo::BSONDoc>>("results");
  TEST(docs.size(), >, count);

  // Important so that we can keep track of async funcs that finished
  repeaterSearchAttempts = 0;
  attemptClose();
}

bool NetworkApi::attemptClose()
{
  for (const auto& attempt : attempts)
  {
    if (attempt != 0)
      return false;
  }

  close();
  return true;
}

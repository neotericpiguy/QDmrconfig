#include <iostream>

#include "BSONDoc.hpp"

static uint32_t passCount = 0;
static uint32_t failCount = 0;

#define TEST_EQ(lparam, rparam)                            \
  if (lparam == rparam)                                    \
  {                                                        \
    std::cout << "\e[32m[PASS]\e[0m "                      \
              << __PRETTY_FUNCTION__ << ":" << __LINE__    \
              << " " << #lparam << " == " << #rparam << "" \
              << std::endl;                                \
    passCount++;                                           \
  }                                                        \
  else                                                     \
  {                                                        \
    std::cout << "\e[31m[FAIL]\e[0m "                      \
              << __PRETTY_FUNCTION__ << ":" << __LINE__    \
              << " " << #lparam << " == " << #rparam       \
              << std::endl;                                \
    failCount++;                                           \
  }

#define START_TEST()                                       \
  std::cout << "\n\e[35m" << __PRETTY_FUNCTION__           \
            << "\n=======================================" \
            << "=========================================" \
            << "\e[0m" << std::endl;

std::string str = "{ \"status\" : \"OK\", \"Licenses\" : { \"page\" : 1, \"rowPerPage\" : \"100\", \"totalRows\" : \"1\", \"lastUpdate\" : \"Dec 19, 2020\", \"License\" : [ { \"licName\" : \"Annua, Jonathan Lee O\", \"frn\" : \"0029046729\", \"callsign\" : \"KJ7LEY\", \"categoryDesc\" : \"Personal Use\", \"serviceDesc\" : \"Amateur\", \"statusDesc\" : \"Active\", \"expiredDate\" : \"12/12/2029\", \"licenseID\" : \"4232049\", \"licDetailURL\" : \"http://wireless2.fcc.gov/UlsApp/UlsSearch/license.jsp?__newWindow=false&licKey=4232049\" } ] } }";
bool simpleDoc()
{
  START_TEST();
  bson_t* b;
  bson_iter_t iter;
  bson_iter_t baz;

  b = BCON_NEW("foo", "{", "bar", "[", "{", "baz", BCON_INT32(1), "}", "]", "}");
  Mongo::BSONDoc results(b);
  std::cout << "b: " << results.toString() << std::endl;

  if (bson_iter_init(&iter, b) &&
      bson_iter_find_descendant(&iter, "foo.bar.0.baz", &baz) &&
      BSON_ITER_HOLDS_INT32(&baz))
  {
    printf("baz = %d\n", bson_iter_int32(&baz));
  }
  Mongo::BSONDoc results2(str);
  std::cout << "results2: " << results2.toString() << std::endl;

  std::vector<Mongo::BSONDoc> licenses;
  results2.getDocuments(licenses, "Licenses.License");
  std::cout << "Licenses: " << licenses.size() << std::endl;
  std::cout << "licenses.size() " << licenses.size() << std::endl;
  if (licenses.size() != 2)
  {
    std::cout << "FAILED" << std::endl;
  }
  return true;
}

bool bsondocVectorTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);

  std::cout << "results: " << results.toString() << std::endl;

  if (results.hasField("status") && results.getString("status") == "OK")
  {
    std::cout << "results.count(): " << results.count() << std::endl;
    TEST_EQ(results.count(), 2);
    std::cout << "has Licenses: " << results.hasField("Licenses") << std::endl;
    std::cout << "has Licenses.lastUpdate: " << results.getString("Licenses.lastUpdate") << std::endl;
    std::cout << "has Licenses.page: " << results.hasField("Licenses.page") << std::endl;
    std::cout << "has Licenses.License: " << results.hasField("Licenses.License") << std::endl;

    std::vector<Mongo::BSONDoc> licenses;
    results.getDocuments(licenses, "Licenses.0.License.licName");
    std::cout << "Licenses: " << licenses.size() << std::endl;
    std::cout << "licenses.size() " << licenses.size() << std::endl;
    TEST_EQ(licenses.size(), 0);
    return true;
  }
  return false;
}

bool keyTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  auto keys = results.getKeys();

  std::cout << "keys.size(): " << keys.size() << std::endl;
  for (const auto& key : keys)
  {
    std::cout << key << ": " << results.isDocument(key) << std::endl;
  }

  TEST_EQ(keys.size(), 2);
  return false;
}

bool getDocumentTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  auto keys = results.getKeys();

  std::cout << "keys.size(): " << keys.size() << std::endl;
  TEST_EQ(keys.size(), 2);
  for (const auto& key : keys)
  {
    std::cout << key << ": " << results.isDocument(key) << std::endl;
    if (results.isDocument(key))
    {
      Mongo::BSONDoc licenses = results.get<Mongo::BSONDoc>(key);
      std::cout << licenses.toString() << std::endl;
      TEST_EQ(licenses.hasField("page"), true);
      TEST_EQ(licenses.get<int32_t>("page"), 1);
    }
  }

  return false;
}

bool templateTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  TEST_EQ(results.has("status"), true);
  TEST_EQ(results.get<std::string>("status"), "OK");
  TEST_EQ(results.has("Licenses"), true);
  TEST_EQ(results.has("Licenses.page"), true);
  // http://mongoc.org/libbson/current/bson_type_t.html
  TEST_EQ(results.getType("Licenses"), 0x3);
  TEST_EQ(results.getType("status"), 0x2);
  TEST_EQ(results.getType("Licenses.page"), 0x10);

  return false;
}

int main()
{
  bsondocVectorTest();
  simpleDoc();
  keyTest();
  templateTest();
  getDocumentTest();

  std::cout << "\e[32m" << passCount << " PASS\e[0m " << std::endl;
  std::cout << "\e[31m" << failCount << " FAIL\e[0m " << std::endl;
  return 0;
}

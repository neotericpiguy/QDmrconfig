#include <iostream>
#include <sstream>

#include "BSONDoc.hpp"

static uint32_t passCount = 0;
static uint32_t failCount = 0;
static std::vector<std::string> failFuncs;

#define TEST(lparam, op, rparam)                                \
  if (lparam == rparam)                                         \
  {                                                             \
    std::cout << "\e[32m[PASS]\e[0m "                           \
              << __PRETTY_FUNCTION__ << ":" << __LINE__         \
              << " " << #lparam << " " << #op << " " << #rparam \
              << std::endl;                                     \
    passCount++;                                                \
  }                                                             \
  else                                                          \
  {                                                             \
    std::stringstream ss;                                       \
    ss << "\e[31m[FAIL]\e[0m "                                  \
       << __PRETTY_FUNCTION__ << ":" << __LINE__                \
       << " " << #lparam << " " << #op << " " << #rparam        \
       << " -> " << lparam << " " << #op << " " << #rparam;     \
    std::cout << ss.str() << std::endl;                         \
    failFuncs.push_back(ss.str());                              \
    failCount++;                                                \
  }

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

  std::vector<Mongo::BSONDoc> licenses = results2.get<std::vector<Mongo::BSONDoc>>("Licenses.License");
  TEST(licenses.size(), ==, 1);
  return true;
}

bool bsondocVectorTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);

  std::cout << "results: " << results.toString() << std::endl;

  TEST(results.has("status"), ==, true);
  TEST(results.getString("status"), ==, "OK");
  TEST(results.count(), ==, 2);
  TEST(results.has("Licenses"), ==, true);
  TEST(results.get<std::string>("Licenses.lastUpdate"), ==, "Dec 19, 2020");
  TEST(results.has("Licenses.page"), ==, true);
  TEST(results.has("Licenses.License"), ==, true);

  std::vector<Mongo::BSONDoc> licenses = results.get<std::vector<Mongo::BSONDoc>>("Licenses.License");
  TEST(licenses.size(), ==, 1);
  return true;
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

  TEST(keys.size(), ==, 2);
  return false;
}

bool getDocumentTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  auto keys = results.getKeys();

  std::cout << "keys.size(): " << keys.size() << std::endl;
  TEST(keys.size(), ==, 2);
  for (const auto& key : keys)
  {
    std::cout << key << ": " << results.isDocument(key) << std::endl;
    if (results.isDocument(key))
    {
      Mongo::BSONDoc licenses = results.get<Mongo::BSONDoc>(key);
      std::cout << licenses.toString() << std::endl;
      TEST(licenses.has("page"), ==, true);
      TEST(licenses.get<int32_t>("page"), ==, 1);
    }
  }

  return false;
}

bool templateTest()
{
  START_TEST();

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  TEST(results.has("status"), ==, true);
  TEST(results.get<std::string>("status"), ==, "OK");
  TEST(results.has("Licenses"), ==, true);
  TEST(results.has("Licenses.page"), ==, true);
  // http://mongoc.org/libbson/current/bson_type_t.html
  TEST(results.getType("Licenses"), ==, 0x3);
  TEST(results.getType("status"), ==, 0x2);
  TEST(results.getType("Licenses.page"), ==, 0x10);

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
  for (const auto& func : failFuncs)
    std::cout << func << std::endl;

  return failFuncs.size() > 0;
}

#include <iostream>

#include "BSONDoc.hpp"
#include "SimpleTest.hpp"

std::string str = "{ \"status\" : \"OK\", \"Licenses\" : { \"page\" : 1, \"rowPerPage\" : \"100\", \"totalRows\" : \"1\", \"lastUpdate\" : \"Dec 19, 2020\", \"License\" : [ { \"licName\" : \"Annua, Jonathan Lee O\", \"frn\" : \"0029046729\", \"callsign\" : \"KJ7LEY\", \"categoryDesc\" : \"Personal Use\", \"serviceDesc\" : \"Amateur\", \"statusDesc\" : \"Active\", \"expiredDate\" : \"12/12/2029\", \"licenseID\" : \"4232049\", \"licDetailURL\" : \"http://wireless2.fcc.gov/UlsApp/UlsSearch/license.jsp?__newWindow=false&licKey=4232049\" },{\"licName\" :\"another\"} ] } }";
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
  TEST(licenses.size(), ==, 2);
  TEST(licenses[0].get<std::string>("frn"), ==, "0029046729");
  TEST(licenses[1].get<std::string>("licName"), ==, "another");

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
  TEST(licenses.size(), ==, 2);
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
  keyTest();
  templateTest();
  getDocumentTest();
  simpleDoc();

  std::cout << SimpleTest::getReport() << std::endl;
  return !SimpleTest::isSuccess();
}

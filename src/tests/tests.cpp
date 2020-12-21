#include <iostream>

#include "BSONDoc.hpp"

bool simpleDoc()
{
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
  std::string str = "{\"Licenses\" : {\"License\" : [{\"licName\" : \"Jonathan\"},{\"licName\":\"another\"}], \"other\" : \"asdf\"}}";
  Mongo::BSONDoc results2(str);
  std::cout << "results2: " << results2.toString() << std::endl;

  std::vector<Mongo::BSONDoc> licenses;
  results2.getDocuments(licenses, "Licenses.License");
  std::cout << "Licenses: " << licenses.size() << std::endl;
  std::cout << "licenses.size() " << licenses.size() << std::endl;
  return true;
}

bool bsondocVectorTest()
{
  std::string str = "{ \"status\" : \"OK\", \"Licenses\" : { \"page\" : \"1\", \"rowPerPage\" : \"100\", \"totalRows\" : \"1\", \"lastUpdate\" : \"Dec 19, 2020\", \"License\" : [ { \"licName\" : \"Annua, Jonathan Lee O\", \"frn\" : \"0029046729\", \"callsign\" : \"KJ7LEY\", \"categoryDesc\" : \"Personal Use\", \"serviceDesc\" : \"Amateur\", \"statusDesc\" : \"Active\", \"expiredDate\" : \"12/12/2029\", \"licenseID\" : \"4232049\", \"licDetailURL\" : \"http://wireless2.fcc.gov/UlsApp/UlsSearch/license.jsp?__newWindow=false&licKey=4232049\" } ] } }";

  Mongo::BSONDoc results(str);

  std::cout << "results: " << results.toString() << std::endl;

  if (results.hasField("status") && results.getString("status") == "OK")
  {
    std::cout << "results.count(): " << results.count() << std::endl;
    std::cout << "has Licenses: " << results.hasField("Licenses") << std::endl;
    std::cout << "has Licenses.lastUpdate: " << results.getString("Licenses.lastUpdate") << std::endl;
    std::cout << "has Licenses.page: " << results.hasField("Licenses.page") << std::endl;
    std::cout << "has Licenses.License: " << results.hasField("Licenses.License") << std::endl;

    std::vector<Mongo::BSONDoc> licenses;
    results.getDocuments(licenses, "Licenses.0.License.licName");
    std::cout << "Licenses: " << licenses.size() << std::endl;
    std::cout << "licenses.size() " << licenses.size() << std::endl;
    return true;
  }
  return false;
}

bool keyTest()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::string str = "{ \"status\" : \"OK\", \"Licenses\" : { \"page\" : \"1\", \"rowPerPage\" : \"100\", \"totalRows\" : \"1\", \"lastUpdate\" : \"Dec 19, 2020\", \"License\" : [ { \"licName\" : \"Annua, Jonathan Lee O\", \"frn\" : \"0029046729\", \"callsign\" : \"KJ7LEY\", \"categoryDesc\" : \"Personal Use\", \"serviceDesc\" : \"Amateur\", \"statusDesc\" : \"Active\", \"expiredDate\" : \"12/12/2029\", \"licenseID\" : \"4232049\", \"licDetailURL\" : \"http://wireless2.fcc.gov/UlsApp/UlsSearch/license.jsp?__newWindow=false&licKey=4232049\" } ] } }";

  Mongo::BSONDoc results(str);
  std::cout << "results: " << results.toString() << std::endl;

  auto keys = results.getKeys();

  std::cout << "keys.size(): " << keys.size() << std::endl;
  for (const auto& key : keys)
  {
    std::cout << key << std::endl;
  }

  return false;
}

int main()
{
  std::cout << "Tests" << std::endl;
  if (!bsondocVectorTest())
  {
    std::cout << "bsondocVectorTest FAILED" << std::endl;
  }
  if (!simpleDoc())
  {
    std::cout << "bsondocVectorTest FAILED" << std::endl;
  }
  keyTest();
  return 0;
}

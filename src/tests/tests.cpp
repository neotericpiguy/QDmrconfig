#include <iostream>

#include "BSONDoc.hpp"

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
    results.getDocuments(licenses, "Licenses.License");
    std::cout << "Licenses: " << licenses.size() << std::endl;
    std::cout << "licenses.size() " << licenses.size() << std::endl;
    return true;
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
  return 0;
}

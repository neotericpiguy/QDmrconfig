#ifndef BSONDOCTESTS_HPP
#define BSONDOCTESTS_HPP

#include "BSONDoc.hpp"
#include "SimpleTest.hpp"

class BSONDocTests : public SimpleTest
{
public:
  BSONDocTests();
  BSONDocTests(const BSONDocTests&) = delete;
  ~BSONDocTests();

  bool simpleDoc();
  bool bsondocVectorTest();
  bool keyTest();
  bool getDocumentTest();
  bool templateTest();
  bool repeaterResultsTest();

private:
  static const std::string str;
  static const std::string repeaterStr;
};
#endif

#ifndef BSONDOCTESTS_HPP
#define BSONDOCTESTS_HPP

#include "BSONDoc.hpp"
#include "RepeaterBookTests.hpp"
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
  bool inlineChangeTest();

  static const std::string str;
};
#endif

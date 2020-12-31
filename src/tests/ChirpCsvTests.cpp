#include "ChirpCsvTests.hpp"

#include "BSONDoc.hpp"

ChirpCsvTests::ChirpCsvTests()
{
  ADD_TEST(ChirpCsvTests::openParse);
}

ChirpCsvTests::~ChirpCsvTests()
{
}

bool ChirpCsvTests::openParse()
{
  ChirpCsv chirpCsv;
  TEST(chirpCsv.open("./examples/not_exist_File"), ==, false);
  TEST(chirpCsv.open("./examples/Baofeng_UV-5R_20200529.csv"), ==, true);
  TEST(chirpCsv.size(), ==, 124);
  auto docs = chirpCsv.getAnalogFormat();
  TEST(docs.size(), ==, 124);
  return true;
}


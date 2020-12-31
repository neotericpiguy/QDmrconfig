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

  // Have the necessary keys to perform a append
  std::vector<std::string> keys = {"Analog", "Name", "Receive", "Transmit", "Power", "Scan", "TOT", "RO", "Admit", "Squelch", "RxTone", "TxTone", "Width", "#"};
  std::sort(keys.begin(), keys.end());

  auto docKeys = docs[0].getKeys();
  std::sort(docKeys.begin(), docKeys.end());
  TEST(std::equal(keys.begin(), keys.end(), docKeys.begin()), ==, true);

  return true;
}

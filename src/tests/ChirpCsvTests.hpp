#ifndef CHIRPCSVTESTS_HPP
#define CHIRPCSVTESTS_HPP

#include "ChirpCsv.hpp"
#include "SimpleTest.hpp"

class ChirpCsvTests : public SimpleTest
{
public:
  ChirpCsvTests();
  ChirpCsvTests(const ChirpCsvTests&) = delete;
  ~ChirpCsvTests();

  bool openParse();
};
#endif

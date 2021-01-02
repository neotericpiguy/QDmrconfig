#ifndef REPEATERBOOKTESTS_HPP
#define REPEATERBOOKTESTS_HPP

#include "RepeaterBook.hpp"
#include "SimpleTest.hpp"

class RepeaterBookTests : public SimpleTest
{
public:
  RepeaterBookTests();
  RepeaterBookTests(const RepeaterBookTests&) = delete;
  ~RepeaterBookTests();

public:
  static const std::string repeaterStr;
  static const std::string tucsonResults;
  static const std::string bensonResults;
  static const std::string vailResults;
  bool openParse();
  bool append();
};
#endif

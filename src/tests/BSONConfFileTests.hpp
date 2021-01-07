#ifndef BSONCONFFILETESTS
#define BSONCONFFILETESTS

#include "BSONConfFile.hpp"
#include "SimpleTest.hpp"

class BSONConfFileTests : public SimpleTest
{
public:
  BSONConfFileTests();
  BSONConfFileTests(const BSONConfFileTests&) = delete;
  ~BSONConfFileTests();

  bool btechSimpleOpen();
  bool rd5rSimpleOpen();
};
#endif

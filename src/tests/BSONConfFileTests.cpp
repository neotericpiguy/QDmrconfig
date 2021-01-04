#include "BSONConfFileTests.hpp"

BSONConfFileTests::BSONConfFileTests()
{
  ADD_TEST(BSONConfFileTests::simpleDoc);
}

BSONConfFileTests::~BSONConfFileTests()
{
}

bool BSONConfFileTests::simpleDoc()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  TEST(bsonConfFile.loadFile("./examples/btech-6x2.conf"), ==, true);
  TEST(bsonConfFile.size(), ==, 14);

  std::cout << bsonConfFile.toString() << std::endl;
  return true;
}


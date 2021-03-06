#include "BSONConfFileTests.hpp"

BSONConfFileTests::BSONConfFileTests()
{
  ADD_TEST(BSONConfFileTests::btechSimpleOpen);
  ADD_TEST(BSONConfFileTests::rd5rSimpleOpen);
  ADD_TEST(BSONConfFileTests::rd5rSimpleWrite);
}

BSONConfFileTests::~BSONConfFileTests()
{
}

bool BSONConfFileTests::btechSimpleOpen()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  bsonConfFile.loadFile("./examples/btech-6x2.conf");
  TEST(bsonConfFile.size(), ==, 14);

  bsonConfFile.saveFile("build/" + std::string(__FUNCTION__) + ".conf");
  return true;
}

bool BSONConfFileTests::rd5rSimpleOpen()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  bsonConfFile.loadFile("./src/dmrconfig/examples/rd5r-factory.conf");
  TEST(bsonConfFile.size(), ==, 8);

  bsonConfFile.saveFile("build/" + std::string(__FUNCTION__) + ".conf");
  return true;
}

bool BSONConfFileTests::rd5rSimpleWrite()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  bsonConfFile.loadFile("./src/dmrconfig/examples/rd5r-factory.conf");
  TEST(bsonConfFile.size(), ==, 8);

  bsonConfFile.saveFile("build/" + std::string(__FUNCTION__) + ".conf");
  return true;
}

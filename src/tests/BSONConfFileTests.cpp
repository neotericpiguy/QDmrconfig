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
  TEST(bsonConfFile.loadFile("./examples/btech-6x2.conf"), ==, true);
  TEST(bsonConfFile.size(), ==, 14);

  std::ofstream temp("build/" + std::string(__FUNCTION__) + ".json");

  temp << bsonConfFile.toString() << std::endl;
  return true;
}

bool BSONConfFileTests::rd5rSimpleOpen()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  TEST(bsonConfFile.loadFile("./src/dmrconfig/examples/rd5r-factory.conf"), ==, true);
  TEST(bsonConfFile.size(), ==, 8);

  std::ofstream temp("build/" + std::string(__FUNCTION__) + ".json");

  temp << bsonConfFile.toString() << std::endl;
  return true;
}

bool BSONConfFileTests::rd5rSimpleWrite()
{
  BSONConfFile bsonConfFile;
  TEST(bsonConfFile.loadFile("examples/not_a_real_file"), ==, false);
  TEST(bsonConfFile.loadFile("./src/dmrconfig/examples/rd5r-factory.conf"), ==, true);
  TEST(bsonConfFile.size(), ==, 8);

  bsonConfFile.saveFile("build/" + std::string(__FUNCTION__) + ".json");
  return true;
}

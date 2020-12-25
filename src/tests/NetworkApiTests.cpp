#include "NetworkApiTests.hpp"

NetworkApiTests::NetworkApiTests()
{
  ADD_TEST(NetworkApiTests::simple);
}

NetworkApiTests::~NetworkApiTests()
{
}

bool NetworkApiTests::simple()
{
  return true;
}

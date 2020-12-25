#ifndef NETWORKAPITESTS_HPP
#define NETWORKAPITESTS_HPP

#include "BSONDoc.hpp"
#include "SimpleTest.hpp"

class NetworkApiTests : public SimpleTest
{
public:
  NetworkApiTests();
  NetworkApiTests(const NetworkApiTests&) = delete;
  ~NetworkApiTests();

  bool simple();
};
#endif

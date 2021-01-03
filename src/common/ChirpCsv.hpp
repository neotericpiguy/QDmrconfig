#ifndef CHIRPCSV_HPP
#define CHIRPCSV_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "BSONDoc.hpp"
#include "ChannelSource.hpp"
#include "StringThings.hpp"

class ChirpCsv : public ChannelSource
{
public:
  ChirpCsv();
  virtual ~ChirpCsv();

  virtual bool open(const std::string& file) override;
  virtual std::vector<Mongo::BSONDoc> getAnalogFormat() const override;
};

#endif

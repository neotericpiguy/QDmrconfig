#ifndef CHIRPCSV_HPP
#define CHIRPCSV_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "BSONDoc.hpp"
#include "StringThings.hpp"

class ChirpCsv
{
public:
  ChirpCsv();
  ~ChirpCsv();

  bool open(const std::string& file);
  const std::vector<Mongo::BSONDoc>& getEntries() const;

  void resize(size_t n);
  size_t size() const;
  std::vector<Mongo::BSONDoc> getAnalogFormat() const;

private:
  std::string _filename;
  std::vector<Mongo::BSONDoc> _entries;
};

#endif

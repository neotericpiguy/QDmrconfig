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
  ChirpCsv(const ChirpCsv&) = delete;
  ~ChirpCsv();

  bool open(const std::string& file);
  const std::vector<Mongo::BSONDoc>& getEntries() const;

  size_t size() const;

private:
  std::string _filename;
  std::vector<Mongo::BSONDoc> _entries;
};

#endif

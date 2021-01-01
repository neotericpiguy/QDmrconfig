#ifndef REPEATERBOOK_HPP
#define REPEATERBOOK_HPP

#include <iostream>
#include <string>
#include <vector>

#include "BSONDoc.hpp"
#include "StringThings.hpp"

class RepeaterBook
{
public:
  RepeaterBook();
  ~RepeaterBook();

  bool open(const std::string& file);
  bool fromStdString(const std::string& results);
  const std::vector<Mongo::BSONDoc>& getEntries() const;

  bool append(const std::string& results);
  void resize(size_t n);
  size_t size() const;
  std::vector<Mongo::BSONDoc> getAnalogFormat() const;

private:
  std::string _filename;
  std::vector<Mongo::BSONDoc> _entries;
};

#endif

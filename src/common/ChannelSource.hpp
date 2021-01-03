#ifndef CHANNELSOURCE_HPP
#define CHANNELSOURCE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "BSONDoc.hpp"
#include "StringThings.hpp"

class ChannelSource
{
public:
  ChannelSource();
  virtual ~ChannelSource() = 0;

  virtual bool open(const std::string& file);
  virtual bool fromStdString(const std::string& results);
  virtual std::vector<Mongo::BSONDoc> getAnalogFormat() const = 0;
  virtual bool append(const std::string& results) = 0;

  const std::vector<Mongo::BSONDoc>& getEntries() const;
  void resize(size_t n);
  size_t size() const;
  void clear();

  bool removeDuplicates(const std::string& key);

protected:
  std::vector<Mongo::BSONDoc> _entries;
};

#endif

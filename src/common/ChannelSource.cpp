#include "ChannelSource.hpp"

ChannelSource::ChannelSource() :
    _entries({})
{
}

ChannelSource::~ChannelSource()
{
}

bool ChannelSource::open(const std::string& file)
{
  return false;
}

bool ChannelSource::fromStdString(const std::string& results)
{
  return false;
}

const std::vector<Mongo::BSONDoc>& ChannelSource::getEntries() const
{
  return _entries;
}

bool ChannelSource::removeDuplicates(const std::string& key)
{
  Mongo::BSONDoc::removeDuplicates(_entries, key);
  return true;
}

size_t ChannelSource::size() const
{
  return _entries.size();
}

void ChannelSource::resize(size_t n)
{
  _entries.resize(n);
}

void ChannelSource::clear()
{
  _entries.clear();
}


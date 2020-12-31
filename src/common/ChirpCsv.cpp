#include "ChirpCsv.hpp"

ChirpCsv::ChirpCsv() :
    _filename(""),
    _entries({})
{
}

ChirpCsv::~ChirpCsv()
{
}

bool ChirpCsv::open(const std::string& file)
{
  _filename = file;
  std::ifstream fs(file);

  if (!fs.is_open())
    return false;

  _entries.clear();
  std::vector<std::string> keys;
  std::string line;
  while (std::getline(fs, line))
  {
    auto row = StringThings::strToVec(line, ',');
    if (keys.empty())
    {
      keys = row;
    }
    else
    {
      Mongo::BSONDoc temp;
      for (unsigned int column = 0; column < row.size(); ++column)
      {
        temp.append(keys[column], row[column]);
      }
      _entries.push_back(temp);
    }
  }

  return true;
}

const std::vector<Mongo::BSONDoc>& ChirpCsv::getEntries() const
{
  return _entries;
}

size_t ChirpCsv::size() const
{
  return _entries.size();
}

void ChirpCsv::resize(size_t n)
{
  _entries.resize(n);
}

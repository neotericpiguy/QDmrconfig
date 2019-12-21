#include "ConfBlock.hpp"

ConfBlock::ConfBlock() :
    _header(),
    _lines(),
    _comments(),
    _columnStart(),
    _columnName(),
    _isTable(false),
    _isModified(false)
{
}

void ConfBlock::replace(std::string& str, const std::string& from, const std::string& to)
{
  size_t start_pos = str.find(from);
  if (start_pos != std::string::npos)
    str.replace(start_pos, from.length(), to);
}

void ConfBlock::setLines(const std::vector<std::string>& lines)
{
  if (lines.size() == 0)
    return;

  _header = lines[0];

  ConfBlock::replace(_header, "# Table of ", "");
  ConfBlock::replace(_header, "# ", "");

  if (_header == "#")
    _header = "Info";

  _isTable = false;
  for (const auto& line : lines)
  {
    if (line[0] == '#')
    {
      _comments.push_back(line);
      continue;
    }

    if (line == "")
      continue;

    //    std::cout << line << std::endl;
    if (line[0] != ' ')  // Header line
    {
      if (line.find(':') == std::string::npos)
      {
        _columnName = strToVec(line, ' ');

        _header = _columnName[0];

        _isTable = true;

        auto lastIndex = 0;
        for (const auto& c : _columnName)
        {
          auto newIndex = line.find(c, lastIndex);
          _columnStart.push_back(newIndex - lastIndex);
          lastIndex = newIndex;
          //std::cout << c << " : " << _columnStart.back() << std::endl;
        }
      }
      else
        _lines.push_back(strToVec(line, ' '));
    }
    else
      _lines.push_back(strToVec(line, ' '));
  }

  _columnStart.push_back(0);

  return;
}

std::string ConfBlock::getHeader() const
{
  return _header;
}

std::string ConfBlock::getConfLines(bool withComments) const
{
  std::string result;
  std::stringstream ss(result);

  if (withComments)
  {
    for (const auto& comment : _comments)
      ss << comment << "\n";
  }

  if (_isTable)
  {
    const auto& line = getColumnNames();
    for (unsigned int i = 0; i < line.size(); i++)
    {
      ss << std::left << std::setw(_columnStart[i + 1]) << line[i];
    }
    ss << std::endl;
  }

  for (unsigned int j = 0; j < getRowCount(); j++)
  {
    const auto& line = _lines.at(j);
    if (!_isTable)
    {
      ss << vecToStr(line, " ") << std::endl;
      continue;
    }

    for (unsigned int i = 0; i < line.size(); i++)
    {
      // i = column
      if (i == 0)
        ss << std::right;
      else
        ss << std::left;

      // j = row
      if (i == 0)
      {
        ss << std::setw(5);
        ss << line[i] << std::string(_columnStart[i + 1] - 5, ' ');
      }
      else
      {
        ss << std::setw(_columnStart[i + 1]);
        ss << line[i];
      }
    }
    ss << std::endl;
  }

  return ss.str();
}

std::vector<std::string> ConfBlock::strToVec(const std::string& vec, char seperator)
{
  if (vec.size() == 0)
    return {};

  std::string tempVal;
  std::istringstream vecStream(vec);
  std::vector<std::string> result;

  while (std::getline(vecStream, tempVal, seperator))
  {
    if (!tempVal.empty())
      result.push_back(tempVal);
  }

  //  for (std::string item; std::getline(vecStream, item, seperator);)
  //  {
  //    std::stringstream tempStream(item);
  //    tempStream >> tempVal;
  //    if(!tempVal.empty())
  //      result.push_back(tempVal);
  //  }

  return result;
}

std::string ConfBlock::vecToStr(const std::vector<std::string>& vec, const std::string& seperator)
{
  if (vec.size() == 0)
    return "";

  std::stringstream ss;
  auto iter = vec.begin();

  ss << *iter;

  iter++;

  for (; iter != vec.end(); iter++)
    ss << seperator << *iter;

  return ss.str();
}

unsigned int ConfBlock::getRowCount() const
{
  return _lines.size();
}

unsigned int ConfBlock::getColumnCount() const
{
  return _columnStart.size() - 1;
}

const std::vector<std::string>& ConfBlock::getColumnNames() const
{
  return _columnName;
}

bool ConfBlock::isTable() const
{
  return _isTable;
}

void ConfBlock::setModified(bool state)
{
  _isModified = state;
}

bool ConfBlock::isModified() const
{
  return _isModified;
}

std::vector<std::vector<std::string>>& ConfBlock::getLines()
{
  return _lines;
}

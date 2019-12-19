#include "ConfBlock.hpp"

ConfBlock::ConfBlock()
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

  ConfBlock::replace(_header,"# Table of ","");
  ConfBlock::replace(_header,"# ","");

  if (_header == "#")
    _header = "Info";

  for (const auto& line : lines)
  {
    if (line[0] == '#')
      continue;

    if(line == "")
      continue;

//    std::cout << line << std::endl;
    if (line[0] != ' ')  // Header line
    {
      if (line.find(':') < line.find(' '))
      {
        std::cout << "not table" << std::endl;
      }
      else
      {
        _columnName = strToVec(line, ' ');
        auto lastIndex = 0;
        for (const auto& c : _columnName)
        {
          auto newIndex = line.find(c, lastIndex);
          _columnStart.push_back(newIndex - lastIndex);
          lastIndex = newIndex;

//          std::cout << c << " : " << _columnStart.back() << std::endl;
        }
      }
    }
    _lines.push_back(strToVec(line, ' '));
  }

  return;
}

std::string ConfBlock::getHeader() const
{
  return _header;
}

std::string ConfBlock::getLines() const
{
  std::string result;
  //  for (const auto& line : _lines)
  //    result += vecToStr(line, " ") + "\n";
  //
  std::stringstream ss(result);


//  for (const auto& line : _lines)
  for (unsigned int j = 0 ; j< _lines.size();j++)
  {
    const auto& line = _lines.at(j);
    if (_columnStart.size() != line.size())
      return "header";

    for (unsigned int i = 0; i < line.size(); i++)
    {
      if(i==0 && j > 0)
        ss << std::right;
      else
        ss << std::left;

      if(i != line.size() -1)
        ss << std::setw(_columnStart[i+1]);
      if(i==0 && j > 0)
        ss << std::setw(5);

      if (i == 0 && j > 0)
        ss << line[i] << std::string(_columnStart[i + 1] - 5, ' ');
      else
        ss << line[i];
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

  while(std::getline(vecStream, tempVal, seperator))
  {
    if(!tempVal.empty())
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

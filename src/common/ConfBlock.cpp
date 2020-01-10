#include "ConfBlock.hpp"

#include <algorithm>

std::map<std::string, ConfBlock*> ConfBlock::_confBlocks;

ConfBlock::ConfBlock() :
    _header(),
    _lines(),
    _comments(),
    _columnStart(),
    _columnName(),
    _valueMap(),
    _isTable(false),
    _isModified(false),
    _metaIndex(0xBEEF)
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
    //Ignore blank lines
    if (line.empty())
      continue;

    // Handle comments
    if (line[0] == '#')
    {
      _comments.push_back(line);
      continue;
    }

    if (line[0] != ' ')  // Header line
    {
      if (line.find(':') != std::string::npos &&
          line.find("Range") == std::string::npos)
      {
        // Use a map
        auto key = line.substr(0, line.find(':'));
        auto value = line.substr(line.find(':') + 2, line.length() - 2 - line.find(':'));
        _valueMap[key] = value;
      }
      else
      {
        // Use a table
        _columnName = strToVec(line, ' ');

        _header = _columnName[0];

        _isTable = true;

        auto lastIndex = 0;
        for (const auto& c : _columnName)
        {
          auto newIndex = line.find(c, lastIndex);
          _columnStart.push_back(newIndex - lastIndex);
          lastIndex = newIndex;
        }

        auto metaIter = std::find(_columnName.begin(), _columnName.end(), "#");
        if (metaIter != _columnName.end())
          _metaIndex = std::distance(_columnName.begin(), metaIter);
      }
    }
    else
    {
      // A table row
      _lines.push_back(strToVec(line, ' '));
      _lines.back().resize(_columnStart.size());
    }
  }

  _columnStart.push_back(0);

  _confBlocks[getHeader()] = this;

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

  if (isTable())
  {
    const auto& line = getColumnNames();
    for (unsigned int i = 0; i < line.size(); i++)
    {
      ss << std::left << std::setw(_columnStart[i + 1]) << line[i];
    }
    ss << std::endl;

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
  }
  else
  {
    for (const auto& [key, value] : _valueMap)
    {
      ss << key << ": " << value << std::endl;
    }
  }

  return ss.str();
}

int ConfBlock::getMetaIndex() const
{
  return _metaIndex;
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

int ConfBlock::getColumnIndex(const std::string& columnName) const
{
  auto iter = std::find(_columnName.begin(), _columnName.end(), columnName);
  if (iter == _columnName.end())
    return -1;

  return std::distance(_columnName.begin(), iter);
}

bool ConfBlock::isTable() const
{
  return _isTable;
}

void ConfBlock::metaUpdate()
{
  auto& rows = getRows();
  std::vector<std::string> rangeUpdates;
  for (unsigned int row = 0; row < rows.size(); row++)
  {
    for (unsigned int column = getMetaIndex() + 1; column < getColumnCount(); column++)
    {
      std::string metaColumnName = getColumnNames()[column];
      std::string targetColumn = metaColumnName;

      if (targetColumn.find("Range") != std::string::npos &&
          std::find(rangeUpdates.begin(), rangeUpdates.end(), targetColumn) == rangeUpdates.end())
      {
        rangeUpdates.push_back(targetColumn);
        std::string srcBlockColumn = targetColumn.substr(targetColumn.rfind("-") + 1, targetColumn.length() - 1 - targetColumn.rfind("-"));
        std::string srcBlockHeader = targetColumn;

        srcBlockHeader = srcBlockHeader.substr(srcBlockHeader.find("Range-") + 6, srcBlockHeader.length() - 6 - srcBlockHeader.find("Range-"));
        ConfBlock::replace(srcBlockHeader, "-" + srcBlockColumn, "");

        std::string dstBlockColumn = targetColumn.substr(0, targetColumn.find("Range"));

        if (_confBlocks.find(srcBlockHeader) != _confBlocks.end())
        {
          ConfBlock& sourceBlock = *(_confBlocks.at(srcBlockHeader));
          updateChannelList(sourceBlock, srcBlockColumn, *this, dstBlockColumn);
        }
      }
      else if (targetColumn.find("Offset") != std::string::npos)
      {
        ConfBlock::replace(targetColumn, "Offset", "");
        // auto newValue = _tableWidget->item(row, column)->text().toStdString();
        auto newValue = rows[row][column];

        int targetColumnIndex = getColumnIndex(targetColumn);
        if (targetColumnIndex != -1)
        {
          if (newValue.find("+") != std::string::npos ||
              newValue.find("-") != std::string::npos)
          {
            int currentVal;
            ConfBlock::strTo(rows[row - 1][targetColumnIndex], currentVal);

            int delta;
            ConfBlock::strTo(newValue, delta);

            auto newOffsetValue = std::to_string(currentVal + delta);
            rows[row][targetColumnIndex] = newOffsetValue;
          }
          else
          {
            rows[row][targetColumnIndex] = newValue;
          }
        }
      }
    }
  }
}

void ConfBlock::setModified(bool state)
{
  _isModified = state;
}

bool ConfBlock::isModified() const
{
  return _isModified;
}

std::vector<std::vector<std::string>>& ConfBlock::getRows()
{
  return _lines;
}

const std::vector<std::vector<std::string>>& ConfBlock::getLines() const
{
  return _lines;
}

std::map<std::string, std::string>& ConfBlock::getMap()
{
  return _valueMap;
}

int ConfBlock::insertRow(int rowIndex, const std::vector<std::string>& newRow)
{
  auto row = newRow;
  auto linesIter = _lines.begin();

  row.resize(getColumnCount());
  std::advance(linesIter, rowIndex);

  _lines.insert(linesIter, row);
  return 0;
}

int ConfBlock::removeRow(int rowIndex)
{
  auto linesIter = _lines.begin();
  std::advance(linesIter, rowIndex);
  _lines.erase(linesIter);
  return 0;
}

std::vector<std::string>& ConfBlock::getRow(int i)
{
  return _lines[i];
}

template <typename T>
std::string ConfBlock::vecToStr(const std::vector<T>& vec, const std::string& seperator)
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
template std::string ConfBlock::vecToStr<int>(const std::vector<int>& vec, const std::string& seperator);

std::string ConfBlock::rangify(std::vector<int>& vec)
{
  std::sort(vec.begin(), vec.end());

  std::vector<std::string> result;
  auto vecIter = vec.begin();
  int startRange = *vecIter;
  int rangeCounter = startRange;

  vecIter++;
  for (; vecIter != vec.end(); vecIter++)
  {
    if (*vecIter == rangeCounter + 1)
    {
      rangeCounter++;
      if (vecIter == vec.end() - 1)
      {
        if (startRange != rangeCounter)
          result.push_back(std::to_string(startRange) + "-" + std::to_string(rangeCounter));
        else
          result.push_back(std::to_string(startRange));
      }
    }
    else
    {
      if (startRange != rangeCounter)
        result.push_back(std::to_string(startRange) + "-" + std::to_string(rangeCounter));
      else
        result.push_back(std::to_string(startRange));

      startRange = *vecIter;
      rangeCounter = startRange;

      if (vecIter == vec.end() - 1)
      {
        result.push_back(std::to_string(*vecIter));
      }
    }
  }

  if (result.size())
    return vecToStr(result, ",");
  return "";
}

void ConfBlock::updateChannelList(const ConfBlock& sourceBlock, const std::string& sourceColumn, ConfBlock& destBlock, const std::string& destColumn)
{
  std::map<std::string, std::vector<int>> scanIndexToChanMap;

  auto sourceColumnIndex = sourceBlock.getColumnIndex(sourceColumn);
  for (const auto& sourceRow : sourceBlock.getLines())  // use getLines because it returns a const vector
  {
    const auto& scanlistNumber = sourceRow[sourceColumnIndex];
    int channel;
    if (ConfBlock::strTo(sourceRow[0], channel))
    {
      scanIndexToChanMap[scanlistNumber].push_back(channel);
    }
  }

  auto destColumnIndex = destBlock.getColumnIndex(destColumn);
  for (auto& destRow : destBlock.getRows())  // use getLines because it returns a const vector
  {
    std::string range = "";
    if (scanIndexToChanMap.find(destRow[0]) != scanIndexToChanMap.end())
    {
      range = ConfBlock::rangify(scanIndexToChanMap[destRow[0]]);
    }

    destRow[destColumnIndex] = range;
  }
  return;
}

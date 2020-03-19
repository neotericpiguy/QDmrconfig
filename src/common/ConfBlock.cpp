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

std::string ConfBlock::getConfLines(bool withComments)
{
  std::string result;
  std::stringstream ss(result);

  if (withComments)
  {
    for (const auto& comment : _comments)
      ss << comment << "\n";
  }

  std::vector<int> columnStart = _columnStart;

  std::cout << getHeader() << " - col: " << columnStart.size() << std::endl;
  if (isTable())
  {
    const auto& line = getColumnNames();
    for (unsigned int i = 2; i < line.size(); i++)
    {
      columnStart[i + 1] = line[i].size() + 1;
    }

    for (unsigned int j = 0; j < getRowCount(); j++)
    {
      const auto& line = _lines.at(j);
      for (unsigned int i = 0; i < line.size(); i++)
      {
        if (columnStart[i + 1] < (int)line[i].size() + 1)
        {
          columnStart[i + 1] = line[i].size() + 1;
        }
      }
    }

    // Write header
    for (unsigned int i = 0; i < line.size(); i++)
      ss << std::left << std::setw(columnStart[i + 1]) << line[i];
    ss << std::endl;

    for (unsigned int j = 0; j < getRowCount(); j++)
    {
      const auto& line = _lines.at(j);

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
          ss << line[i] << std::string(columnStart[i + 1] - 5, ' ');
        }
        else
        {
          ss << std::setw(columnStart[i + 1]);
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
        std::map<ConfBlock*, std::string> srcBlocks;
        std::string dstBlockColumn = targetColumn.substr(0, targetColumn.find("Range"));

        std::string temp = targetColumn;
        ConfBlock::replace(temp, dstBlockColumn + "Range-", "");

        while (temp.find("-") != std::string::npos && !temp.empty())
        {
          auto delim = temp.find("-");
          if (delim == std::string::npos)
            break;

          auto endDelim = temp.find("-", delim + 1);
          if (endDelim == std::string::npos)
            endDelim = temp.length();

          std::string srcBlockHeader = temp.substr(0, delim);
          std::string srcBlockColumn = temp.substr(delim + 1, endDelim - delim - 1);

          if (_confBlocks.find(srcBlockHeader) != _confBlocks.end())
          {
            auto sourceBlock = _confBlocks.at(srcBlockHeader);
            srcBlocks[sourceBlock] = srcBlockColumn;
          }

          ConfBlock::replace(temp, srcBlockHeader + "-" + srcBlockColumn, "");
          if (!temp.empty() && temp[0] == '-')
            temp = temp.substr(1, temp.length() - 1);
        }

        if (!srcBlocks.empty())
          updateChannelList(srcBlocks, *this, dstBlockColumn);
      }
      else if (targetColumn.find("Offset") != std::string::npos)
      {
        ConfBlock::replace(targetColumn, "Offset", "");
        // auto newValue = _tableWidget->item(row, column)->text().toStdString();
        auto newValue = rows[row][column];

        int targetColumnIndex = getColumnIndex(targetColumn);
        int temp;
        if (targetColumnIndex != -1)
        {
          if (newValue[0] == '+' ||
              newValue[0] == '-')
          {
            int currentVal;
            ConfBlock::strTo(rows[row - 1][targetColumnIndex], currentVal);

            int delta;
            ConfBlock::strTo(newValue, delta);

            auto newOffsetValue = std::to_string(currentVal + delta);
            rows[row][targetColumnIndex] = newOffsetValue;
          }
          else if (ConfBlock::strTo(newValue, temp))
          {
            rows[row][targetColumnIndex] = newValue;
          }
          else
          {
            std::string column = newValue.substr(0, newValue.find("-"));
            std::string srcHeader = newValue.substr(newValue.find("-") + 1, newValue.length() - 1 - newValue.find("-"));
            std::string srcColumnStr = srcHeader.substr(srcHeader.find("-") + 1, srcHeader.length() - 1 - srcHeader.find("-"));
            ConfBlock::replace(srcHeader, "-" + srcColumnStr, "");

            if (_confBlocks.find(srcHeader) != _confBlocks.end())
            {
              const ConfBlock& srcBlock = *_confBlocks.at(srcHeader);
              int srcColumn = srcBlock.getColumnIndex(srcColumnStr);
              int max = 0;
              if (srcColumn != -1)
              {
                int temp;
                for (const auto& line : srcBlock.getLines())
                {
                  if (ConfBlock::strTo(line[srcColumn], temp))
                  {
                    if (temp > max)
                      max = temp;
                  }
                }
              }
              rows[row][getColumnIndex(column)] = std::to_string(max + 1);
            }
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
  if (vec.size() == 1)
    return std::to_string(vec[0]);

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

void ConfBlock::updateChannelList(const std::map<ConfBlock*, std::string>& src, ConfBlock& destBlock, const std::string& destColumn)
{
  std::map<std::string, std::vector<int>> scanIndexToChanMap;

  for (const auto& [sourceBlockPtr, sourceColumn] : src)
  {
    auto& sourceBlock = *sourceBlockPtr;
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

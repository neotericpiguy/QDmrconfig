#ifndef CONFBLOCK_H
#define CONFBLOCK_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class ConfBlock
{
public:
  ConfBlock();

  void setLines(const std::vector<std::string>& lines);
  std::string getHeader() const;
  std::string getConfLines(bool withComments = true) const;
  int getMetaIndex() const;

  // Table getter/setters
  const std::vector<std::vector<std::string>>& getLines() const;
  std::vector<std::vector<std::string>>& getRows();
  std::vector<std::string>& getRow(int);
  const std::vector<std::string>& getColumnNames() const;
  int getColumnIndex(const std::string& columnName) const;
  unsigned int getRowCount() const;
  unsigned int getColumnCount() const;
  int insertRow(int, const std::vector<std::string>&);
  int removeRow(int);

  // Map getter/setter
  std::map<std::string, std::string>& getMap();

  void setModified(bool state);

  bool isModified() const;
  bool isTable() const;

  template <typename T>
  static bool strTo(const std::string& tempStr, T& retval)
  {
    std::istringstream iss(tempStr);

    iss >> retval;
    if (iss.fail())
      return false;
    return true;
  }
  static void replace(std::string& str, const std::string& from, const std::string& to);
  static std::string rangify(std::vector<int>& vec);
  static std::vector<std::string> strToVec(const std::string& vec, char seperator);
  template <typename T>
  static std::string vecToStr(const std::vector<T>& vec, const std::string& seperator);

private:
  std::string _header;
  std::vector<std::vector<std::string>> _lines;
  std::vector<std::string> _comments;

  std::vector<int> _columnStart;
  std::vector<std::string> _columnName;

  std::map<std::string, std::string> _valueMap;

  bool _isTable;
  bool _isModified;
  int _metaIndex;
};

#endif

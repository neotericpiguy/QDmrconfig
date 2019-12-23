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

  // Table getter/setters
  std::vector<std::vector<std::string>>& getRows();
  std::vector<std::string>& getRow(int);
  const std::vector<std::string>& getColumnNames() const;
  unsigned int getRowCount() const;
  unsigned int getColumnCount() const;
  int insertRow(int, const std::vector<std::string>&);
  int removeRow(int);

  // Map getter/setter
  std::map<std::string, std::string>& getMap();

  void setModified(bool state);

  bool isModified() const;
  bool isTable() const;

  static void replace(std::string& str, const std::string& from, const std::string& to);
  static std::vector<std::string> strToVec(const std::string& vec, char seperator);
  static std::string vecToStr(const std::vector<std::string>& vec, const std::string& seperator);

private:
  std::string _header;
  std::vector<std::vector<std::string>> _lines;
  std::vector<std::string> _comments;

  std::vector<int> _columnStart;
  std::vector<std::string> _columnName;

  std::map<std::string, std::string> _valueMap;

  bool _isTable;
  bool _isModified;
};

#endif

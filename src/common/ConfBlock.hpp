#ifndef CONFBLOCK_H
#define CONFBLOCK_H

#include <fstream>
#include <iomanip>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

class ConfBlock
{
public:
  ConfBlock();

  void setLines(const std::vector<std::string>& lines);
  std::string getHeader() const;
  std::string getLines() const;

  static void replace(std::string& str, const std::string& from, const std::string& to);
  static std::vector<std::string> strToVec(const std::string& vec, char seperator);
  static std::string vecToStr(const std::vector<std::string>& vec, const std::string& seperator);

private:
  std::string _header;
  std::vector<std::vector<std::string>> _lines;

  std::vector<int> _columnStart;
  std::vector<int> _columnWidth;
  std::vector<std::string> _columnName;
};

#endif

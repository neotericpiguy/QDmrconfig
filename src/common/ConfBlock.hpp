#ifndef CONFBLOCK_HPP
#define CONFBLOCK_HPP

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
  std::string getConfLines(bool withComments = true);
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

  void metaUpdate();
  void updateChannelList(const std::map<ConfBlock*, std::string>& src, ConfBlock& destBlock, const std::string& destColumn);

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
  static std::vector<int> unrangify(const std::string& range);
  static std::vector<std::string> strToVec(const std::string& vec, char seperator);
  template <typename T>
  static std::string vecToStr(const std::vector<T>& vec, const std::string& seperator);
  //  static std::optional<std::string> replaceRegex(const std::string& string, const std::string& search, const std::string& replace)
  //  {
  //    std::regex searchRegex;
  //
  //    try
  //    {
  //      searchRegex = std::regex(search, std::regex::ECMAScript);
  //    }
  //    catch (const std::exception& e)
  //    {
  //      return {};
  //    }
  //
  //    return std::regex_replace(string, searchRegex, replace);
  //  }

  static void replaceRegex(std::string& string, const std::string& search, const std::string& replace)
  {
    std::regex searchRegex;

    try
    {
      searchRegex = std::regex(search, std::regex::ECMAScript);
    }
    catch (const std::exception& e)
    {
      return;
    }

    string = std::regex_replace(string, searchRegex, replace);
    return;
  }

  static std::vector<std::smatch> getCaptures(const std::regex& regex, const std::string& search)
  {
    //Good example for the performance difference between
    //Initialization and instanciation and assignment
    //  auto begin = std::sregex_iterator(search.begin(), search.end(), regex);
    //  auto end = std::sregex_iterator();
    std::sregex_iterator begin(search.begin(), search.end(), regex);
    std::sregex_iterator end;

    std::vector<std::smatch> result;

    for (auto i = begin; i != end; i++)
      result.push_back(*i);
    return result;
  }
  static bool getRegex(std::regex& regex, const std::string& regexStr)
  {
    try
    {
      regex = std::regex(regexStr, std::regex::ECMAScript);
    }
    catch (const std::exception& e)
    {
      //      std::cout << DEBUG_ERROR << " " << regexStr << std::endl;
      return false;
    }
    return true;
  }

private:
  std::string _header;
  std::vector<std::vector<std::string>> _lines;
  std::vector<std::string> _comments;

  std::vector<int> _columnStart;
  std::vector<std::string> _columnName;

  std::map<std::string, std::string> _valueMap;

  static std::map<std::string, ConfBlock*> _confBlocks;

  bool _isTable;
  bool _isModified;
  int _metaIndex;
};

#endif

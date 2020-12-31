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

#include "BSONDoc.hpp"
#include "StringThings.hpp"

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

  // BSONDocs
  bool appendRepeaterDoc(const std::vector<Mongo::BSONDoc>& docs);

  // Map getter/setter
  std::map<std::string, std::string>& getMap();

  void setModified(bool state);

  bool isModified() const;
  bool isTable() const;

  void metaUpdate();
  void updateChannelList(const std::map<ConfBlock*, std::string>& src, ConfBlock& destBlock, const std::string& destColumn);

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

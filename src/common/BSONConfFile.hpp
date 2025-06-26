#ifndef BSONCONFFILE_HPP
#define BSONCONFFILE_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "BSONDoc.hpp"

class BSONConfFile
{
public:
  BSONConfFile();

  bool loadFile(const std::string& filename);
  bool saveFile(const std::string& filename);
  void uploadFile();
  void downloadFile(const std::string& filename);

  bool isModified() const;

  std::string toString() const;
  size_t size() const;

  bool sortChannelDocs(const std::string& key);

  static std::string vecToTable(const std::vector<Mongo::BSONDoc>& docs, unsigned int columns);

  Mongo::BSONDoc& getConfDoc();
  std::map<std::string, Mongo::BSONDoc>& getConfDocs();
  std::vector<Mongo::BSONDoc>& getChannelDocs();

private:
  Mongo::BSONDoc _confDoc;
  std::map<std::string, Mongo::BSONDoc> _confDocs;
  std::map<std::string, std::map<std::string, std::string>> _confmaps;
  std::vector<Mongo::BSONDoc> _channelDocs;
};

#endif

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

private:
  Mongo::BSONDoc _confDoc;
  std::map<std::string, Mongo::BSONDoc> _confDocs;
  std::vector<Mongo::BSONDoc> channelDocs;
};

#endif

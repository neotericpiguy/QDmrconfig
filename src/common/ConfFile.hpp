#ifndef CONFFILE_H
#define CONFFILE_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "ConfBlock.hpp"

class ConfFile
{
public:
  ConfFile();

  void loadFile(const std::string& filename);
  void saveFile();
  void uploadFile();

  void updateChannelList(const ConfBlock& sourceBlock, const std::string& sourceColumn, ConfBlock& destBlock, const std::string& destColumn);

  bool isModified() const;

  std::map<int, ConfBlock>& getConfBlocks();
  std::map<std::string, ConfBlock*>& getNameBlocks();

private:
  std::string _filename;
  std::map<int, ConfBlock> _confBlocks;
  std::map<std::string, ConfBlock*> _confNameBlocks;
};

#endif

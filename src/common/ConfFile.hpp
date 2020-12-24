#ifndef CONFFILE_HPP
#define CONFFILE_HPP

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
  void downloadFile(const std::string& filename);

  bool isModified() const;

  std::map<int, ConfBlock>& getConfBlocks();
  std::map<std::string, ConfBlock*>& getNameBlocks();

private:
  std::string _filename;
  std::map<int, ConfBlock> _confBlocks;
  std::map<std::string, ConfBlock*> _confNameBlocks;
};

#endif

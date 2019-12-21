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

  bool isModified() const;

  std::map<int, ConfBlock>& getConfBlock();

private:
  std::string _filename;
  std::map<int, ConfBlock> _confBlocks;
};

#endif

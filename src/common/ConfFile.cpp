#include "ConfFile.hpp"

#include "QDmrconfigEditor.h"

ConfFile::ConfFile() :
    _filename(),
    _confBlocks(),
    _confNameBlocks()
{
}

void ConfFile::loadFile(const std::string& filename)
{
  _confBlocks.clear();
  _confNameBlocks.clear();

  std::map<int, std::vector<std::string>> blocks;

  int blockId = 0;
  std::ifstream inStream(filename);

  while (!inStream.eof())
  {
    std::string line;
    std::getline(inStream, line);

    if (!line.empty())
      blocks[blockId].push_back(line);

    if (line == "")
      blockId++;
  }

  for (const auto& [key, value] : blocks)
    _confBlocks[key].setLines(value);

  for (auto& [key, value] : _confBlocks)
    _confNameBlocks[value.getHeader()] = &value;

  _filename = filename;
}

std::map<int, ConfBlock>& ConfFile::getConfBlocks()
{
  return _confBlocks;
}

std::map<std::string, ConfBlock*>& ConfFile::getNameBlocks()
{
  return _confNameBlocks;
}

void ConfFile::saveFile()
{
  std::ofstream outStream(_filename);
  for (auto& [index, confBlock] : _confBlocks)
  {
    outStream << confBlock.getConfLines() << std::endl;
    confBlock.setModified(false);
  }
}

bool ConfFile::isModified() const
{
  for (auto& [index, confBlock] : _confBlocks)
    if (confBlock.isModified())
      return true;
  return false;
}

void ConfFile::uploadFile()
{
  if (isModified())
    saveFile();

  radio_connect();
  radio_download();
  radio_print_version(stdout);
  radio_save_image("backup.img");
  radio_parse_config(_filename.c_str());
  radio_verify_config();
  radio_upload(1);
  radio_disconnect();
}


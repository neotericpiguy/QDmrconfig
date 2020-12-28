#include "ConfFile.hpp"

ConfFile::ConfFile(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile) :
    _filename(),
    _confBlocks(),
    _confNameBlocks(),
    _radioUploadFile(radioUploadFile),
    _radioDownloadFile(radioDownloadFile)
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

  for (const auto& pair : blocks)
  {
    const auto& key = pair.first;
    const auto& value = pair.second;
    _confBlocks[key].setLines(value);
  }

  for (auto& pair : _confBlocks)
  {
    //    const auto& key = pair.first;
    auto& value = pair.second;
    _confNameBlocks[value.getHeader()] = &value;
  }

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

  for (auto& pair : _confBlocks)
  {
    //    const auto& index = pair.first;
    auto& confBlock = pair.second;
    confBlock.metaUpdate();
  }

  for (auto& pair : _confBlocks)
  {
    auto& confBlock = pair.second;
    confBlock.metaUpdate();
    outStream << confBlock.getConfLines() << std::endl;
    confBlock.setModified(false);
  }
}

bool ConfFile::isModified() const
{
  for (auto& pair : _confBlocks)
  {
    auto& confBlock = pair.second;
    if (confBlock.isModified())
      return true;
  }
  return false;
}

void ConfFile::uploadFile()
{
  if (isModified())
    saveFile();

  _radioUploadFile(_filename);
}

void ConfFile::downloadFile(const std::string& filename)
{
  _radioDownloadFile(filename);
}

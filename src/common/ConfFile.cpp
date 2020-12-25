#include "ConfFile.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" const char version[] = VERSION;
const char* copyright;

extern char* optarg;
extern int optind;

extern "C" void radio_connect(int);
extern "C" void radio_download(void);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_print_version(FILE* out);
extern "C" void radio_disconnect(void);
extern "C" void radio_save_image(const char* filename);
extern "C" void radio_print_config(FILE* out, int verbose);
extern "C" void radio_parse_config(const char* filename);
extern "C" void radio_verify_config();
extern "C" void radio_upload(int cont_flag);

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

  radio_connect(-1);
  radio_download();
  radio_print_version(stdout);
  radio_save_image("backup.img");
  radio_parse_config(_filename.c_str());
  radio_verify_config();
  radio_upload(1);
  radio_disconnect();
}

void ConfFile::downloadFile(const std::string& filename)
{
  std::string imageFilename = filename;
  ConfBlock::replace(imageFilename, ".conf", ".img");

  radio_connect(-1);
  radio_download();
  radio_print_version(stdout);
  radio_disconnect();
  radio_save_image(imageFilename.c_str());

  std::cout << "Print configuration to file " << filename << std::endl;

  FILE* conf = fopen(filename.c_str(), "w");
  if (!conf)
  {
    perror(filename.c_str());
    exit(-1);
  }
  radio_print_config(conf, 1);
  fclose(conf);
}

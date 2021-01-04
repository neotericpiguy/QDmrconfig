#include "BSONConfFile.hpp"

BSONConfFile::BSONConfFile()
{
}

void BSONConfFile::loadFile(const std::string& /*filename*/)
{
}

//std::map<int, ConfBlock>& BSONConfFile::getConfBlocks()
//{
//  return _confBlocks;
//}
//
//std::map<std::string, ConfBlock*>& BSONConfFile::getNameBlocks()
//{
//  return _confNameBlocks;
//}

void BSONConfFile::saveFile()
{
}

bool BSONConfFile::isModified() const
{
  return false;
}

void BSONConfFile::uploadFile()
{
}

void BSONConfFile::downloadFile(const std::string& /*filename*/)
{
}

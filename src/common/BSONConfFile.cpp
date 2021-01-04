#include "BSONConfFile.hpp"

BSONConfFile::BSONConfFile() :
    _confDoc()
{
}

bool BSONConfFile::loadFile(const std::string& filename)
{
  std::ifstream file(filename);

  if (!file.is_open())
    return false;

  _confDoc.clear();

  Mongo::BSONDoc currentDoc;
  std::string currentDocName;
  std::string line;
  int32_t docCount = 0;
  while (std::getline(file, line))
  {
    // line is a comment
    if (line[0] == '#' && currentDocName.empty() && line.length() > 2)
    {
      currentDocName = line.substr(2, line.length() - 2);
    }

    // End of block
    if (line.empty() || file.eof())
    {
      _confDoc.append(currentDocName, currentDoc);

      currentDocName = "";
      currentDoc.clear();
      docCount++;
    }
  }
  _confDoc.append("count", docCount);

  return true;
}

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

std::string BSONConfFile::toString() const
{
  return _confDoc.toString();
}

size_t BSONConfFile::size() const
{
  return _confDoc.get<int64_t>("count");
}

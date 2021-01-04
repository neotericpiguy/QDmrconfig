#include "BSONConfFile.hpp"

#include "StringThings.hpp"

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
  Mongo::BSONDoc descDoc;
  Mongo::BSONDoc mapDoc;
  std::vector<Mongo::BSONDoc> entryDoc;
  std::vector<std::string> headerVec;

  std::string currentDocName;
  std::string line;
  int32_t docCount = 0;
  while (std::getline(file, line))
  {
    // line is a comment
    auto columns = StringThings::strToVec(line, ' ');
    if (line[0] == '#')
    {
      if (currentDocName.empty() && line.length() > 2)
      {
        currentDocName = line.substr(2, line.length() - 2);
      }

      //# 1) Channel number: 1-4000
      if (line.find(")") != std::string::npos && line.find(":") != std::string::npos)
      {
        // Comment is a description
        auto start = line.find(")");
        auto end = line.find(":");
        auto key = line.substr(start + 2, end - start - 2);
        auto desc = line.substr(end + 2, line.length() - end - 2);
        descDoc.append(key, desc);
      }
    }
    // Line is a key value pair
    else if (line.find(":") != std::string::npos)
    {
      // Radio: BTECH DMR-6x2
      auto end = line.find(":");
      auto key = line.substr(0, end);
      auto desc = line.substr(end + 2, line.length() - end - 2);
      mapDoc.append(key, desc);
    }
    else if (columns.size() >= descDoc.count())
    {
      if (headerVec.empty())
        headerVec = columns;
      else
      {
        Mongo::BSONDoc temp;
        for (unsigned int i = 0; i < columns.size(); ++i)
        {
          temp.append(headerVec[i], columns[i]);
        }
        entryDoc.push_back(temp);
      }
    }

    // End of block
    if (line.empty() || file.eof())
    {
      if (!descDoc.empty())
        currentDoc.append("Descriptions", descDoc);
      if (!mapDoc.empty())
        currentDoc.append("Map", mapDoc);
      if (!entryDoc.empty())
        currentDoc.append("Entires", entryDoc);
      _confDoc.append(currentDocName, currentDoc);

      currentDocName = "";
      currentDoc.clear();
      descDoc.clear();
      mapDoc.clear();
      entryDoc.clear();
      headerVec.clear();
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

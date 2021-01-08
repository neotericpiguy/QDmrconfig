#include "BSONConfFile.hpp"

#include "StringThings.hpp"

BSONConfFile::BSONConfFile() :
    _confDoc(),
    _confDocs(),
    channelDocs()
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
          if (i < headerVec.size())
          {
            double tempNumber;
            if (StringThings::isNumber(columns.at(i)) && StringThings::strTo(tempNumber, columns.at(i)))
            {
              temp.append(headerVec[i], tempNumber);
            }
            else
            {
              temp.append(headerVec[i], columns.at(i));
            }
          }
        }
        entryDoc.push_back(temp);

        if (temp.has("Analog") || temp.has("Digital"))
        {
          if (temp.has("Analog"))
          {
            double channelNumber = temp.get<double>("Analog");
            temp.append("Type", "Analog");
            temp.append("Channel", channelNumber);
          }
          else if (temp.has("Digital"))
          {
            double channelNumber = temp.get<double>("Digital");
            temp.append("Type", "Digital");
            temp.append("Channel", channelNumber);
          }

          channelDocs.push_back(temp);
        }
      }
    }

    // End of block
    if (line.empty() || file.eof())
    {
      if (!descDoc.empty())
        currentDoc.append("Descriptions", descDoc);
      if (!mapDoc.empty())
        currentDoc.append("Map", mapDoc);
      if (!entryDoc.empty() && !entryDoc[0].has("Analog") && !entryDoc[0].has("Digital"))
        currentDoc.append("Entires", entryDoc);

      _confDoc.append(currentDocName, currentDoc);
      _confDocs[currentDocName] = currentDoc;

      currentDocName = "";
      currentDoc.clear();
      descDoc.clear();
      mapDoc.clear();
      entryDoc.clear();
      headerVec.clear();
      docCount++;
    }
  }

  return true;
}

bool BSONConfFile::saveFile(const std::string& filename)
{
  std::ofstream fileStream(filename);
  if (!fileStream.is_open())
    return false;

  std::vector<std::string> confDocKeys = _confDoc.getKeys();
  for (unsigned int keyIndex = 0; keyIndex < confDocKeys.size(); ++keyIndex)
  {
    const auto& key = confDocKeys.at(keyIndex);
    if (_confDocs.find(key) == _confDocs.end())
      continue;

    const auto& doc = _confDocs.at(key);
    if (keyIndex == 0)
      fileStream << "#" << std::endl;
    fileStream << "# " << key << std::endl;
    fileStream << "#" << std::endl;

    if (doc.has("Descriptions"))
    {
      const auto& descDoc = doc.get<Mongo::BSONDoc>("Descriptions");
      const auto descDocKeys = descDoc.getKeys();
      for (unsigned int descDocIndex = 0; descDocIndex < descDocKeys.size(); ++descDocIndex)
      {
        const auto& descDocKey = descDocKeys.at(descDocIndex);
        fileStream << "# " << descDocIndex + 1 << ") " << descDocKey << ": "
                   << descDoc.get<std::string>(descDocKey) << std::endl;
      }
      fileStream << "#\n";
    }

    if (doc.has("Map"))
    {
      const auto& mapDoc = doc.get<Mongo::BSONDoc>("Map");
      for (const auto& mapKey : mapDoc.getKeys())
      {
        fileStream << mapKey << ": " << mapDoc.get<std::string>(mapKey) << std::endl;
      }
      fileStream << std::endl;
    }
    else
    {
      std::vector<Mongo::BSONDoc> tempDocs;
      if (key.find("digital") != std::string::npos)
      {
        for (const auto& channelDoc : channelDocs)
        {
          if (channelDoc.has("Type") && channelDoc.get<std::string>("Type") == "Digital")
          {
            tempDocs.push_back(channelDoc);
          }
        }
        fileStream << vecToTable(tempDocs);
      }
      else if (key.find("analog") != std::string::npos)
      {
        for (const auto& channelDoc : channelDocs)
        {
          if (channelDoc.has("Type") && channelDoc.get<std::string>("Type") == "Analog")
          {
            tempDocs.push_back(channelDoc);
          }
        }
        fileStream << vecToTable(tempDocs);
      }
      fileStream << std::endl;
    }
  }

  sortChannelDocs("Channel");

  return true;
}

bool BSONConfFile::sortChannelDocs(const std::string& key)
{
  std::sort(channelDocs.begin(), channelDocs.end(), [&key](const Mongo::BSONDoc& a, const Mongo::BSONDoc& b) {
    return a.get<double>(key) < b.get<double>(key);
  });
  return true;
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
  return _confDoc.count();
}

std::string BSONConfFile::vecToTable(const std::vector<Mongo::BSONDoc>& docs)
{
  if (docs.empty())
    return "";

  std::stringstream ss;
  std::vector<std::string> keys = docs.at(0).getKeys();
  std::vector<unsigned> columnSizes(keys.size(), 0);

  // Find max column size
  for (unsigned i = 0; i < keys.size(); i++)
  {
    if (keys[i].length() > columnSizes[i])
      columnSizes[i] = keys[i].length();
  }
  for (const auto& doc : docs)
  {
    for (unsigned i = 0; i < keys.size(); i++)
    {
      if (doc.isString(keys[i]))
      {
        std::string value = doc.get<std::string>(keys[i]);
        if (value.length() > columnSizes[i])
        {
          columnSizes[i] = value.length();
        }
      }
    }
  }
  //  ss << StringThings::vecToStr(columnSizes, ", ") << std::endl;

  for (unsigned i = 0; i < keys.size(); i++)
  {
    ss << std::left << std::setw(columnSizes[i] + 1) << keys[i];
  }
  ss << std::endl;

  //  ss << doc.toString() << std::endl;
  for (unsigned i = 0; i < docs.size(); i++)
  {
    const auto& doc = docs.at(i);
    for (unsigned j = 0; j < keys.size(); j++)
    {
      const auto& key = keys.at(j);

      // first column
      if (j == 0)
      {
        ss << std::right;
        ss << std::setw(columnSizes[j] - 1);
      }
      else
      {
        ss << std::left;
        ss << std::setw(columnSizes[j] + 1);
      }

      if (doc.isString(key))
      {
        ss << doc.get<std::string>(key);
      }
      else if (doc.isDouble(key))
      {
        ss << doc.get<double>(key);
      }
      else
      {
        ss << "unk";
      }

      // first column
      if (j == 0)
      {
        ss << "  ";
      }
    }
    ss << std::endl;
  }

  return ss.str();
}

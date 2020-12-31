#include "ChirpCsv.hpp"

ChirpCsv::ChirpCsv() :
    _filename(""),
    _entries({})
{
}

ChirpCsv::~ChirpCsv()
{
}

bool ChirpCsv::open(const std::string& file)
{
  _filename = file;
  std::ifstream fs(file);

  if (!fs.is_open())
    return false;

  _entries.clear();
  std::vector<std::string> keys;
  std::string line;
  while (std::getline(fs, line))
  {
    auto row = StringThings::strToVec(line, ',', false);
    if (keys.empty())
    {
      keys = row;
    }
    else
    {
      Mongo::BSONDoc temp;
      for (unsigned int column = 0; column < row.size(); ++column)
      {
        temp.append(keys[column], row[column]);
      }
      _entries.push_back(temp);
    }
  }

  return true;
}

std::vector<Mongo::BSONDoc> ChirpCsv::getAnalogFormat() const
{
  std::vector<Mongo::BSONDoc> results;
  for (const auto& entry : _entries)
  {
    if (!entry.has("Name") ||
        !entry.has("Frequency") ||
        !entry.has("Duplex") ||
        !entry.has("Offset") ||
        !entry.has("Name"))
      continue;

    Mongo::BSONDoc temp;
    temp.append("Name", entry.get<std::string>("Name"));

    double rxFreq;
    if (!StringThings::strTo(rxFreq, entry.get<std::string>("Frequency")))
      continue;
    temp.append("Receive", StringThings::fixed(rxFreq, 3));

    double offset;
    if (!StringThings::strTo(offset, entry.get<std::string>("Offset")))
      continue;

    std::string duplex = entry.get<std::string>("Duplex");
    if (duplex == "-" || duplex == "+")
      temp.append("Transmit", duplex + StringThings::fixed(offset, 1));
    else
      temp.append("Transmit", "+0.0");

    //Analog  Name             Receive  Transmit Power Scan  TOT RO Admit
    //Squelch RxTone TxTone Width # AnalogOffset Zone
    temp.append("Analog", "");
    temp.append("Power", "High");
    temp.append("Scan", "-");
    temp.append("TOT", "-");
    temp.append("RO", "-");
    temp.append("Squelch", "Normal");
    temp.append("Admit", "-");
    temp.append("Width", "25");
    temp.append("#", "#");

    if (entry.get<std::string>("Tone") == "DTCS")
    {
      std::string code = "D" + entry.get<std::string>("DtcsCode") + entry.get<std::string>("DtcsPolarity")[0];
      temp.append("RxTone", code);
      temp.append("TxTone", code);
    }
    else
    {
      double rTone;
      if (!StringThings::strTo(rTone, entry.get<std::string>("cToneFreq")))
        continue;
      if (rTone != 88.5)
        temp.append("RxTone", StringThings::fixed(rTone, 1));
      else
        temp.append("RxTone", "-");

      double cTone;
      if (!StringThings::strTo(cTone, entry.get<std::string>("rToneFreq")))
        continue;
      if (cTone != 88.5)
        temp.append("TxTone", StringThings::fixed(cTone, 1));
      else
        temp.append("TxTone", "-");
    }
    results.push_back(temp);
  }
  return results;
}

const std::vector<Mongo::BSONDoc>& ChirpCsv::getEntries() const
{
  return _entries;
}

size_t ChirpCsv::size() const
{
  return _entries.size();
}

void ChirpCsv::resize(size_t n)
{
  _entries.resize(n);
}

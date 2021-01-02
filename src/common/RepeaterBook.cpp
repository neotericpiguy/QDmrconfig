#include "RepeaterBook.hpp"

RepeaterBook::RepeaterBook() :
    _filename(""),
    _entries({})
{
}

RepeaterBook::~RepeaterBook()
{
}

bool RepeaterBook::open(const std::string& file)
{
  return false;
}

bool RepeaterBook::fromStdString(const std::string& results)
{
  _entries.clear();
  append(results);

  return true;
}

std::vector<Mongo::BSONDoc> RepeaterBook::getAnalogFormat() const
{
  std::vector<Mongo::BSONDoc> results;
  for (const auto& entry : _entries)
  {
    if (entry.empty())
      continue;

    if (!entry.has("Callsign") ||
        !entry.has("Frequency") ||
        !entry.has("Input Freq") ||
        !entry.has("PL"))
      continue;

    Mongo::BSONDoc temp;
    temp.append("Analog", "");
    temp.append("Name", entry.get<std::string>("Callsign"));

    //Analog  Name             Receive  Transmit Power Scan  TOT RO Admit
    //Squelch RxTone TxTone Width # AnalogOffset Zone

    double rxFreq;
    if (!StringThings::strTo(rxFreq, entry.get<std::string>("Frequency")))
      continue;
    temp.append("Receive", StringThings::fixed(rxFreq, 3));

    double txFreq;
    if (!StringThings::strTo(txFreq, entry.get<std::string>("Input Freq")))
      continue;

    temp.append("Transmit", StringThings::fixed(txFreq - rxFreq, 1));

    temp.append("Power", "High");
    temp.append("Scan", "-");
    temp.append("TOT", "-");
    temp.append("RO", "-");
    temp.append("Admit", "-");
    temp.append("Squelch", "Normal");
    temp.append("Width", "25");
    temp.append("#", "#");
    temp.append("RxTone", "-");

    temp.append("TxTone", entry.get<std::string>("PL"));
    results.push_back(temp);
  }
  return results;
}

const std::vector<Mongo::BSONDoc>& RepeaterBook::getEntries() const
{
  return _entries;
}

bool RepeaterBook::append(const std::string& results)
{
  if (!Mongo::BSONDoc::isValid(results))
    return false;

  Mongo::BSONDoc resultsDoc(results);

  if (!resultsDoc.has("count") || !resultsDoc.has("results"))
    return false;

  int32_t count = resultsDoc.get<int32_t>("count");

  auto entries = resultsDoc.get<std::vector<Mongo::BSONDoc>>("results");
  entries.resize(count);

  _entries.insert(_entries.end(), entries.begin(), entries.end());
  return true;
}

bool RepeaterBook::removeDuplicates(const std::string& key)
{
  Mongo::BSONDoc::removeDuplicates(_entries, key);
  return true;
}

size_t RepeaterBook::size() const
{
  return _entries.size();
}

void RepeaterBook::resize(size_t n)
{
  _entries.resize(n);
}

void RepeaterBook::clear()
{
  _entries.clear();
}

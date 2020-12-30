#include "StringThings.hpp"

namespace StringThings {
std::string fixed(double number, unsigned int decimals)
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(decimals) << number;
  return ss.str();
}

void replaceRegex(std::string& string, const std::string& search, const std::string& replace)
{
  std::regex searchRegex;

  try
  {
    searchRegex = std::regex(search, std::regex::ECMAScript);
  }
  catch (const std::exception& e)
  {
    return;
  }

  string = std::regex_replace(string, searchRegex, replace);
  return;
}

void replace(std::string& str, const std::string& from, const std::string& to)
{
  size_t start_pos = str.find(from);
  if (start_pos != std::string::npos)
    str.replace(start_pos, from.length(), to);
}

std::string rangify(std::vector<int>& vec)
{
  if (vec.size() == 1)
    return std::to_string(vec[0]);

  std::sort(vec.begin(), vec.end());

  std::vector<std::string> result;
  auto vecIter = vec.begin();
  int startRange = *vecIter;
  int rangeCounter = startRange;

  vecIter++;
  for (; vecIter != vec.end(); vecIter++)
  {
    if (*vecIter == rangeCounter + 1)
    {
      rangeCounter++;
      if (vecIter == vec.end() - 1)
      {
        if (startRange != rangeCounter)
          result.push_back(std::to_string(startRange) + "-" + std::to_string(rangeCounter));
        else
          result.push_back(std::to_string(startRange));
      }
    }
    else
    {
      if (startRange != rangeCounter)
        result.push_back(std::to_string(startRange) + "-" + std::to_string(rangeCounter));
      else
        result.push_back(std::to_string(startRange));

      startRange = *vecIter;
      rangeCounter = startRange;

      if (vecIter == vec.end() - 1)
      {
        result.push_back(std::to_string(*vecIter));
      }
    }
  }

  if (result.size())
    return vecToStr(result, ",");
  return "";
}

std::vector<std::string> strToVec(const std::string& vec, char seperator)
{
  if (vec.size() == 0)
    return {};

  std::string tempVal;
  std::istringstream vecStream(vec);
  std::vector<std::string> result;

  while (std::getline(vecStream, tempVal, seperator))
  {
    if (!tempVal.empty())
      result.push_back(tempVal);
  }

  return result;
}

std::vector<int> unrangify(const std::string& range)
{
  std::stringstream ss(range);
  std::vector<int> result;
  int temp;

  if (range == "-")
    return {};
  else if (range.find_first_of(",-") == std::string::npos)
  {
    ss >> temp;
    return {temp};
  }

  while (!ss.eof())
  {
    char c;
    ss >> temp;
    ss >> c;
    if (c == ',')
      result.push_back(temp);
    else if (c == '-')
    {
      int start = temp;
      int end;
      ss >> end;
      for (int i = start; i <= end; i++)
        result.push_back(i);
    }
  }

  return result;
}

template <typename T>
std::string vecToStr(const std::vector<T>& vec, const std::string& seperator)
{
  if (vec.size() == 0)
    return "";

  std::stringstream ss;
  auto iter = vec.begin();

  ss << *iter;

  iter++;

  for (; iter != vec.end(); iter++)
    ss << seperator << *iter;

  return ss.str();
}
template std::string vecToStr<int>(const std::vector<int>& vec, const std::string& seperator);

template <typename T>
bool strTo(T& retval, const std::string& tempStr)
{
  std::istringstream iss(tempStr);

  iss >> retval;
  if (iss.fail())
    return false;
  return true;
}
template bool strTo<int>(int& retval, const std::string& tempStr);
template bool strTo<double>(double& retval, const std::string& tempStr);

}  // namespace StringThings

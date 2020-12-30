#ifndef STRINGTHING_HPP
#define STRINGTHING_HPP

#include <fstream>
#include <iomanip>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace StringThings {

std::string fixed(double number, unsigned int decimals);
void replaceRegex(std::string& string, const std::string& search, const std::string& replace);
void replace(std::string& str, const std::string& from, const std::string& to);

std::string rangify(std::vector<int>& vec);
std::vector<int> unrangify(const std::string& range);

std::vector<std::string> strToVec(const std::string& vec, char seperator);
template <typename T>
std::string vecToStr(const std::vector<T>& vec, const std::string& seperator);

template <typename T>
static bool strTo(const std::string& tempStr, T& retval)
{
  std::istringstream iss(tempStr);

  iss >> retval;
  if (iss.fail())
    return false;
  return true;
}

}  // namespace StringThings
#endif

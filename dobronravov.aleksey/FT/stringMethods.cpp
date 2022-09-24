#include "stringMethods.hpp"

std::string dobronravov::getWord(std::string& str)
{
  if (str.empty())
  {
    return "";
  }
  size_t nextSpace = str.find_first_of(' ');
  std::string word{""};
  if (nextSpace == str.npos)
  {
    word = str;
    str.erase();
  }
  else
  {
    word = str.substr(0, nextSpace);
    str.erase(0, nextSpace + 1);
  }
  return word;
}

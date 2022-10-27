#include "reading.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>

namespace
{
  void removePunctuationMarks(std::string& str)
  {
    if (!std::isalpha(str.front()))
    {
      str.erase(str.begin());
    }
    if (!std::isalpha(str.back()))
    {
      str.erase(str.end() - 1);
    }
  }
}

bazzhin::dictionary bazzhin::readDictionary(std::istream& file)
{
  bazzhin::dictionary dictionary;
  std::string str = "";
  while (file >> str)
  {
    addWordAtDictionary(str, dictionary);
  }
  return dictionary;
}

void bazzhin::addWordAtDictionary(std::string& str, bazzhin::dictionary& dict)
{
  removePunctuationMarks(str);
  bool isOnlyLetters = true;
  for (char i : str)
  {
    if (!std::isalpha(i))
    {
      isOnlyLetters = false;
      break;
    }
  }
  if (isOnlyLetters)
  {
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    if (dict.find(str) != dict.end())
    {
      ++dict.at(str);
    }
    else
    {
      dict.insert(std::make_pair(str, 1));
    }
  }
}

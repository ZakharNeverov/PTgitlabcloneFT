#include "IOActions.hpp"
#include <fstream>
#include <algorithm>

bool maistrenko::getWordFromStream(std::istream& in, std::pair< EngW, RusS >& dest)
{
  std::string commandLine = "";
  std::getline(in, commandLine);

  return getWordFromString(commandLine, dest);
}

bool maistrenko::getWordFromString(std::string& commandLine, std::pair< EngW, RusS >& dest)
{
  bool isInputingGood = true;
  EngW engWord = "";
  isInputingGood = getNextWord(commandLine, ' ', engWord);

  std::string gottenWord = "";
  isInputingGood = getNextWord(commandLine, ' ', gottenWord) && isInputingGood;
  isInputingGood = (gottenWord == "-") && isInputingGood;

  RusS rusSet = RusS();
  while (getNextWord(commandLine, '|', gottenWord))
  {
    rusSet.insert(gottenWord);
  }
  isInputingGood = !rusSet.empty() && isInputingGood;

  if (isInputingGood)
  {
    dest = { engWord, rusSet };
    return true;
  }
  return false;
}

void maistrenko::getDictFromStream(std::istream& in, Dict& dest)
{
  while (!in.eof())
  {
    std::pair< EngW, RusS > dictElem = std::pair< EngW, RusS >();
    if (getWordFromStream(in, dictElem))
    {
      if (dest.find(dictElem.first) == dest.end())
      {
        dest.insert(dictElem);
      }
      else
      {
        RusS tempSet = RusS();

        auto first1 = dest.at(dictElem.first).begin();
        auto last1 = dest.at(dictElem.first).end();
        auto first2 = dictElem.second.begin();
        auto last2 = dictElem.second.end();
        std::set_union(first1, last1, first2, last2, std::inserter(tempSet, tempSet.end()));
        dest.at(dictElem.first) = tempSet;
      }
    }
  }
  in.clear();
}

bool maistrenko::getNextWord(std::string& fromString, char delimiter, std::string& toString)
{
  toString = "";

  size_t closestSpace = fromString.find_first_of(delimiter);
  toString = fromString.substr(0, closestSpace);
  size_t countForDelete = (closestSpace == std::string::npos) ? closestSpace : closestSpace + 1;
  fromString.erase(0, countForDelete);

  return !toString.empty();
}

void maistrenko::outputTranslations(const Dict& dict, const EngW& engWord, std::ostream& out)
{
  RusS rusList = dict.at(engWord);
  bool wasFirstGiven = false;
  for (auto&& word: rusList)
  {
    if (wasFirstGiven)
    {
      out << ", ";
    }
    out << word;
    wasFirstGiven = true;
  }
  out << '\n';
}

void maistrenko::outputDict(const Dict& dict, std::ostream& out)
{
  for (auto&& words: dict)
  {
    out << words.first << " - ";
    outputTranslations(dict, words.first, out);
  }
}

#include "commands.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include "stringMethods.hpp"

namespace
{
  bool isValidDictname(const std::string& str, const dobronravov::dicts_t& dicts)
  {
    return dicts.find(str) != dicts.end();
  }
  bool isValidDictnames(std::string str, const dobronravov::dicts_t& dicts)
  {
    if (str.empty())
    {
      return false;
    }
    while (!str.empty())
    {
      std::string dictName = dobronravov::getWord(str);
      if (!isValidDictname(dictName, dicts))
      {
        return false;
      }
    }
    return true;
  }
  size_t getCountOfWords(const std::string& str)
  {
    return std::count(str.begin(), str.end(), ' ');
  }
}

void dobronravov::doUnion(std::string commands, dicts_t& dicts)
{
  std::string newDictName = getWord(commands);

  if (getCountOfWords(commands) >= 2 && !isValidDictnames(commands, dicts))
  {
    throw std::logic_error("Bad dict names!");
  }

  std::string firstDictName = getWord(commands);
  std::string secondDictName = getWord(commands);

  dict_t newDict = unionDicts(dicts[firstDictName], dicts[secondDictName]);
  std::string nextDictname = getWord(commands);
  while (!nextDictname.empty())
  {
    newDict = unionDicts(newDict, dicts[nextDictname]);
    nextDictname = getWord(commands);
  }

  dicts[newDictName] = newDict;
}

void dobronravov::doIntersect(std::string commands, dicts_t& dicts)
{
  std::string newDictName = getWord(commands);

  if (getCountOfWords(commands) >= 2 && !isValidDictnames(commands, dicts))
  {
    throw std::logic_error("Bad dict names!");
  }

  std::string firstDictName = getWord(commands);
  std::string secondDictName = getWord(commands);

  dict_t newDict = intersectDicts(dicts[firstDictName], dicts[secondDictName]);
  std::string nextDictname = getWord(commands);
  while (!nextDictname.empty())
  {
    newDict = intersectDicts(newDict, dicts[nextDictname]);
    nextDictname = getWord(commands);
  }

  dicts[newDictName] = newDict;
}

void dobronravov::doComplement(std::string commands, dicts_t& dicts)
{
  std::string newDictName = getWord(commands);

  if (getCountOfWords(commands) == 2 && !isValidDictnames(commands, dicts))
  {
    throw std::logic_error("Bad dict names!");
  }

  std::string firstDictName = getWord(commands);
  std::string secondDictName = getWord(commands);

  dicts[newDictName] = complementDicts(dicts[firstDictName], dicts[secondDictName]);
}

void dobronravov::doLoad(std::string commands, dicts_t& dicts)
{
  if (getCountOfWords(commands) == 2)
  {
    throw std::logic_error("Bad dict names!");
  }
  std::string newDictName = getWord(commands);
  std::string filename = getWord(commands);

  std::ifstream fin(filename);

  if (!fin.is_open())
  {
    throw std::logic_error("Bad filename!");
  }

  dicts[newDictName] = readDictFromStream(fin);
}

void dobronravov::doTranslate(std::string commands, dicts_t& dicts, std::ostream& out)
{
  if (getCountOfWords(commands) == 2)
  {
    throw std::logic_error("Bad dict names!");
  }
  std::string newDictName = getWord(commands);
  std::string englishWord = getWord(commands);

  auto&& dict = dicts[newDictName];
  auto iter = dict.find(englishWord);
  if (iter == dict.end())
  {
    throw std::logic_error("Bad english word!");
  }
  else
  {
    outTranslation(*iter, out);
  }
}

void dobronravov::doRemove(std::string commands, dicts_t& dicts)
{
  if (getCountOfWords(commands) == 2)
  {
    throw std::logic_error("Bad dict names!");
  }
  std::string newDictName = getWord(commands);
  std::string englishWord = getWord(commands);

  dicts[newDictName].erase(englishWord);
}

void dobronravov::doCreateDict(std::string commands, dicts_t& dicts)
{
  if (getCountOfWords(commands) == 1)
  {
    throw std::logic_error("Bad dict names!");
  }
  std::string newDictName = getWord(commands);

  dicts[newDictName] = {};
}

void dobronravov::doPrint(std::string commands, dicts_t& dicts, std::ostream& out)
{
  std::string dictName = getWord(commands);
  if (!isValidDictname(dictName, dicts) && !commands.empty())
  {
    throw std::logic_error("Bad args!");
  }

  outDict(dicts.at(dictName), out);
}

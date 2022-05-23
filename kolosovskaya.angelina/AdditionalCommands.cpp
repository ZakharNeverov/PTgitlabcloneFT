#include "AdditionalCommands.h"
#include <iosfwd>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include "IoOperations.h"

namespace kolosovskaya
{
  Dict makeDict(std::istream& in, std::ostream& out)
  {
    std::string str = "";
    std::getline(in, str);
    kolosovskaya::Dict dictionary;
    if (str.empty())
    {
      throw std::logic_error("This dict is empty!\n");
    }
    while (str.size())
    {
      std::string enWord = getPartOfStr(str, ' ');
      size_t enWPos = str.find_first_of("abcdefghijklmnopqrstuvwxyz");
      std::string ruStr = str.substr(0, enWPos);
      size_t delim = (enWPos == std::string::npos) ? enWPos : enWPos + 1;
      str.erase(0, delim - 1);
      dictionary[enWord] = getRuWords(ruStr);
    }
    return dictionary;
  }

  ruW getRuWords(std::string& ruStr)
  {
    kolosovskaya::ruW ruSet;
    while (ruStr.size())
    {
      ruSet.insert(getPartOfStr(ruStr, ','));
      ruStr.erase(0, 1);
    }
    return ruSet;
  }

  std::ostream& printDict(std::ostream& out, const std::string& dictName, const Dicts& allDicts)
  {
    if (!doesExist(dictName, allDicts))
    {
      return printNotExistedDictMessage(out);
    }
    if (allDicts.at(dictName).empty())
    {
      return printEmptyDictMessage(out);
    }
    auto dictIter = allDicts.find(dictName);
    const kolosovskaya::Dict& words = dictIter->second;
    bool isFirst = true;
    for (auto const& words : words)
    {
      if (isFirst)
      {
        out << "Dictionary: " << (*dictIter).first << '\n';
      }
      out << words.first << " - ";
      isFirst = false;
      printRuWords(out, words.second);
      out << '\n';
    }
    return out;
  }

  void printRuWords(std::ostream& out, const ruW& set)
  {
    bool isFirst = true;
    for (auto const& ruWords : set)
    {
      if (isFirst)
      {
        out << ruWords;
      }
      else
      {
        out << ", " << ruWords;
      }
      isFirst = false;
    }
  }

  std::vector< std::string > getNamesFromString(std::string str, char delim)
  {
    std::vector< std::string > dictNames;
    while (!str.empty())
    {
      dictNames.push_back(getPartOfStr(str, delim));
    }
    return dictNames;
  }

  ruW complementRuWords(const ruW& first, const ruW& second)
  {
    kolosovskaya::ruW newSet;
    std::set_difference(first.begin(), first.end(), second.begin(), second.end(), std::inserter(newSet, newSet.end()));
    return newSet;
  }

  Dict complementDicts(const Dict& first, const Dict& second)
  {
    kolosovskaya::Dict newDict = first;
    auto sIter = second.begin();
    kolosovskaya::ruW ruSet;
    while (sIter != second.end())
    {
      if (newDict.find(sIter->first) != newDict.end())
      {
        ruSet = complementRuWords(newDict.find(sIter->first)->second, sIter->second);
        if (ruSet.size() != 0)
        {
          newDict[sIter->first] = ruSet;
        }
        else
        {
          newDict.erase(sIter->first);
        }
      }
      else
      {
        newDict[sIter->first] = sIter->second;
      }
      sIter++;
    }
    return newDict;
  }

  ruW makeUnionRuWords(const ruW& first, const ruW& second)
  {
    kolosovskaya::ruW newSet;
    std::set_union(first.begin(), first.end(), second.begin(), second.end(), std::inserter(newSet, newSet.end()));
    return newSet;
  }

  Dict makeUnion(const Dict& first, const Dict& second)
  {
    kolosovskaya::Dict newDict = first;
    auto sIter = second.begin();
    kolosovskaya::ruW ruSet;
    while (sIter != second.end())
    {
      if (newDict.find(sIter->first) != newDict.end())
      {
        ruSet = makeUnionRuWords(newDict.find(sIter->first)->second, sIter->second);
        newDict[sIter->first] = ruSet;
      }
      else
      {
        newDict[sIter->first] = sIter->second;
      }
      sIter++;
    }
    return newDict;
  }

  bool doesExist(const std::string& dictName, const Dicts& allDicts)
  {
    auto dict = allDicts.find(dictName);
    return dict != allDicts.end();
  }

  Dicts getDicts(std::string& str, std::ostream& out)
  {
    std::vector< std::string > names = getNamesFromString(str, ' ');
    kolosovskaya::Dicts allDicts;
    for (auto& name : names)
    {
      std::ifstream fin(name);
      if (!fin.is_open())
      {
        throw std::logic_error("File isn't open!\n");
      }
      std::string dictName = getPartOfStr(name, '.');
      Dict newDict = makeDict(fin, out);
      if (doesExist(dictName, allDicts))
      {
        throw std::logic_error("The dict is existed yet!\n");
      }
      allDicts[dictName] = newDict;
    }
    return allDicts;
  }
}

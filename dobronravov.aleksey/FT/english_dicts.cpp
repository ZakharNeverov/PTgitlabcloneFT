#include "english_dicts.hpp"
#include <fstream>
#include <algorithm>

#include "stringMethods.hpp"

namespace
{
  bool isStringBraced(const std::string& str)
  {
    if (str.size() < 2)
    {
      return false;
    }
    return str[0] == '{' && str[str.size() - 1] == '}';
  }
}

dobronravov::dict_t dobronravov::readDictFromStream(std::istream& in)
{
  if (!std::istream::sentry(in))
  {
    throw std::logic_error("Bad stream!");
  }

  dict_t dict;
  while (!in.eof())
  {
    std::string str{""};
    std::getline(in, str);
    if (!str.empty())
    {
      word_t englishWord = getWord(str);
      if (isStringBraced(str))
      {
        dict[englishWord] = readRuList(str.substr(1, str.size() - 2));
      }
      else
      {
        throw std::logic_error("Bad dict!");
      }
    }
  }
  return dict;
}

dobronravov::ru_list_t dobronravov::readRuList(std::string str)
{
  ru_list_t ruList;
  while (!str.empty())
  {
    ruList.push_back(getWord(str));
  }
  return ruList;
}

void dobronravov::outDict(const dict_t& dict, std::ostream& out)
{
  for (auto&& translation: dict)
  {
    outTranslation(translation, out);
  }
}

void dobronravov::outTranslation(const dict_t::value_type& translation, std::ostream& out)
{
  out << translation.first << ' ';
  outRuList(translation.second, out);
  out << '\n';
}

void dobronravov::outRuList(const ru_list_t& list, std::ostream& out)
{
  out << '{';
  bool isFirst = true;
  for (auto&& word: list)
  {
    if (!isFirst)
    {
      out << ' ';
    }
    else
    {
      isFirst = false;
    }
    out << word;
  }
  out << '}';
}

dobronravov::dict_t dobronravov::unionDicts(const dict_t& first, const dict_t& second)
{
  dict_t newDict = second;
  for (auto&& translation: first)
  {
    auto tmpIter = newDict.find(translation.first);
    if (tmpIter == newDict.end())
    {
      newDict[translation.first] = translation.second;
    }
    else
    {
      newDict[translation.first] = unionLists(newDict[translation.first], translation.second);
    }
  }
  return newDict;
}

dobronravov::ru_list_t dobronravov::unionLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t newList;
  std::set_union(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(newList));
  return newList;
}

dobronravov::dict_t dobronravov::intersectDicts(const dict_t& first, const dict_t& second)
{
  dict_t newDict;
  for (auto&& firstTran: first)
  {
    auto secondIter = second.find(firstTran.first);
    if (secondIter != second.end())
    {
      newDict[firstTran.first] = intersectLists(firstTran.second, secondIter->second);
    }
  }
  return newDict;
}

dobronravov::ru_list_t dobronravov::intersectLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t newList;
  std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(newList));
  return newList;
}

dobronravov::dict_t dobronravov::complementDicts(const dict_t& first, const dict_t& second)
{
  dict_t newDict;
  for (auto&& firstTran: first)
  {
    auto secondIter = second.find(firstTran.first);
    if (secondIter != second.end())
    {
      newDict[firstTran.first] = complementLists(firstTran.second, secondIter->second);
    }
    else
    {
      newDict[firstTran.first] = firstTran.second;
    }
  }
  return newDict;
}

dobronravov::ru_list_t dobronravov::complementLists(const ru_list_t& first, const ru_list_t& second)
{
  ru_list_t newList;
  std::set_difference(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(newList));
  return newList;
}

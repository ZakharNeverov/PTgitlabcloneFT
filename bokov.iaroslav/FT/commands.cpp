#include "commands.hpp"
#include "strings-parse.hpp"
#include <stdexcept>
#include <fstream>
#include <algorithm>

namespace bokov
{
  void printTranslations(std::set < std::string > translations, std::ostream& out)
  {
    bool flag = true;
    out << '[';
    for (auto& transl : translations)
    {
      if (!flag)
      {
        out << ',';
      }
      flag = false;
      out << transl;
    }
    out << "]\n";
  }

  void printOneDict(const dictionary& dict, std::ostream& out)
  {
    if (dict.empty())
    {
      out << '\n';
    }
    for (auto& ERpair: dict)
    {
      out << ERpair.first << ' ';
      printTranslations(ERpair.second, out);
    }
  }

  void printMax(const dictionary& dict, std::ostream& out)
  {
    if (dict.empty())
    {
      out << '\n';
    }
    size_t max = 0;
    auto nextIter = ++dict.begin();
    for (auto iter = dict.begin(); iter != --dict.end(); ++iter)
    {
      if (iter->second.size() < nextIter->second.size())
      {
        max = nextIter->second.size();
      }
      ++nextIter;
    }
    out << max << '\n';
  }
}

void bokov::printDict(std::string& str, dicts_list& dicts, std::ostream& out)
{
  setlocale(LC_ALL, "rus");
  auto names = bokov::getRowOfWords(str);
  if (names.empty())
  {
    throw std::logic_error("No parameters in printing command!");
    return;
  }
  bool flag = true;
  for (auto& name: names)
  {
    auto dict_iter = dicts.find(name);
    if (dict_iter != dicts.end())
    {
      if (!flag)
      {
        out << '\n';
      }
      flag = false;
      printOneDict(dict_iter->second, out);
    }
  }
  out << '\n';
}

void bokov::loadDict(std::string& str, dicts_list& dicts)
{
  auto dict_file_pair = bokov::getRowOfWords(str);
  if (dict_file_pair.empty())
  {
    throw std::logic_error("No parameters in loading command!");
    return;
  }
  std::string name = dict_file_pair[1];
  bokov::makeDictionary(name, dicts[dict_file_pair[0]]);
}

void bokov::removeElement(std::string& str, dicts_list& dicts)
{
  auto remove_pair = bokov::getRowOfWords(str);
  if (remove_pair.size() < 2)
  {
    throw std::logic_error("Not enough parameters in remove command!");
    return;
  }
  auto dict_iter = dicts.find(remove_pair[0]);
  if (dict_iter != dicts.end())
  {
    auto key_iter = dict_iter->second.find(remove_pair[1]);
    dict_iter->second.erase(key_iter);
  }
}

void bokov::mergeDicts(std::string& str, dicts_list& dicts)
{
  auto list = bokov::getRowOfWords(str);
  if (list.empty())
  {
    throw std::logic_error("No dictionaries to merge!");
    return;
  }
  dictionary buffer;
  for (auto& dict_name: list)
  {
    auto dict_iter = dicts.find(dict_name);
    if (dict_iter != dicts.end())
    {
      buffer.insert(dict_iter->second.begin(), dict_iter->second.end());
    }
  }
  if (!buffer.empty())
  {
    dicts[list[0]] = buffer;
  }
  else
  {
    throw std::logic_error("No dictionaries found to merge!");
  }
}

void bokov::intersectDicts(std::string& str, dicts_list& dicts)
{
  auto params = bokov::getRowOfWords(str);
  if (params.size() < 2)
  {
    throw std::logic_error("Not enough parameters!");
    return;
  }
  dictionary dictBuf;
  auto iter1 = dicts.find(params[1]);
  auto iter2 = dicts.find(params[2]);
  if (iter1 != iter2 && iter1 != dicts.end() && iter2 != dicts.end())
  {
    for (auto& pair1: iter1->second)
    {
      for (auto& pair2: iter2->second)
      {
        if (pair1.first == pair2.first)
        {
          std::set< std::string > buffer;
          std::set_intersection(pair1.second.begin(), pair1.second.end(), pair2.second.begin(), pair2.second.end(),
            std::inserter(buffer, buffer.begin()));
          dictBuf[pair1.first] = buffer;
        }
      }
    }
  }
  if (!dictBuf.empty())
  {
    dicts[params[0]] = dictBuf;
  }
  else
  {
    throw std::logic_error("Nothing to intersect!");
  }
}

void bokov::complementDicts(std::string& str, dicts_list& dicts)
{
  auto params = bokov::getRowOfWords(str);
  if (params.size() < 2)
  {
    throw std::logic_error("Not enough parameters!");
    return;
  }
  dictionary dictBuf;
  auto iter1 = dicts.find(params[1]);
  auto iter2 = dicts.find(params[2]);
  if (iter1 == iter2)
  {
    throw std::logic_error("Difference is null");
    return;
  }
  if (iter1 != dicts.end() && iter2 != dicts.end())
  {
    dictionary sec = iter2->second;
    for (auto& pair1: iter1->second)
    {
      auto it = sec.find(pair1.first);
      if (it == sec.end())
      {
        dictBuf[pair1.first] = pair1.second;
      }
      else
      {
        std::set < std::string > diff;
        std::set_difference(pair1.second.begin(), pair1.second.end(), it->second.begin(), it->second.end(),
          std::inserter(diff, diff.begin()));
        dictBuf[pair1.first] = diff;
      }
    }
  }
  if (!dictBuf.empty())
  {
    dicts[params[0]] = dictBuf;
  }
  else
  {
    throw std::logic_error("No difference!");
  }
}

void bokov::getTop(std::string& str, dicts_list& dicts, std::ostream& out)
{
  if (str.find(' ') != str.npos)
  {
    throw std::invalid_argument("Incorrect parameter!");
  }
  auto dict_iter = dicts.find(str);
  if (dict_iter != dicts.end())
  {
    bokov::printMax(dict_iter->second, out);
  }
  out << '\n';
}

void bokov::insertInDict(std::string& str, dicts_list& dicts)
{
  auto dict_ER = bokov::getRowOfWords(str);
  if (dict_ER.size() < 3)
  {
    throw std::logic_error("Not enough parameters for insertion command!");
    return;
  }
  auto dict_iter = dicts.find(dict_ER[0]);
  std::string e_word = dict_ER[1];
  dict_ER.erase(dict_ER.begin(), dict_ER.begin() + 1);
  if (dict_iter != dicts.end())
  {
    for (auto& translation: dict_ER)
    {
      dict_iter->second[e_word].insert(translation);
    }
  }
}

void bokov::searchInDict(std::string& str, dicts_list& dicts, std::ostream& out)
{
  auto dict_key = bokov::getRowOfWords(str);
  if (dict_key.size() != 2)
  {
    throw std::logic_error("Incorrect count of parameters");
    return;
  }
  auto dict_iter = dicts.find(dict_key[0]);
  if (dict_iter != dicts.end())
  {
    auto key_iter = dict_iter->second.find(dict_key[1]);
    if (key_iter != dict_iter->second.end())
    {
      bokov::printTranslations(key_iter->second, out);
    }
    else
    {
      throw std::invalid_argument("Not such word found!");
    }
  }
  out << '\n';
}

void bokov::deleteDict(std::string& str, dicts_list& dicts)
{
  auto list = bokov::getRowOfWords(str);
  if (list.empty())
  {
    throw std::logic_error("No dictionaries to delete!");
    return;
  }
  for (auto& dict_name: list)
  {
    dicts.erase(dict_name);
  }
}

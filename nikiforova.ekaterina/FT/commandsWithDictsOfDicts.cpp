#include "commandsWithDictsOfDicts.h"
#include <string>
#include <iostream>
#include <fstream>
#include "errorMessages.h"
#include "operationsWithStrings.h"

nikiforova::DictOfDicts nikiforova::readAllDictsFromStream(std::istream& in)
{
  nikiforova::DictOfDicts result;
  while (!in.eof())
  {
    std::string str = "";
    std::getline(in, str);
    if (!str.empty())
    {
      std::string nameOfDict = nikiforova::getWord(str);
      nikiforova::DictOfDicts::const_iterator cIter = result.find(nameOfDict);
      if (cIter != result.cend())
      {
        throw std::logic_error("Dictionary with the same name already exists");
      }
      else
      {
        nikiforova::Dict temp = nikiforova::convertStringToDict(str);
        result.insert(std::make_pair(nameOfDict, temp));
      }
    }
  }
  return result;
}

nikiforova::Dict nikiforova::convertStringToDict(std::string& str)
{
  nikiforova::Dict dict;
  while (!str.empty())
  {
    std::string temp = nikiforova::getWord(str);
    dict.insert(temp);
  }
  return dict;
}

std::ostream& nikiforova::doPrint(std::ostream& out, const std::string& dataset, const Dict& dict)
{
  if (dict.empty())
  {
    nikiforova::emptyMessage(out);
    out << "\n";
    return out;
  }
  out << dataset;
  for (auto&& pair: dict)
  {
    out << " " << pair;
  }
  out << "\n";
  return out;
}

void nikiforova::print(std::string& str, const DictOfDicts& dicts)
{
  std::string nameOfDict = nikiforova::getWord(str);
  nikiforova::DictOfDicts::const_iterator cIter = dicts.find(nameOfDict);
  if (cIter == dicts.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict dict = cIter->second;
  nikiforova::doPrint(std::cout, nameOfDict, dict);
}

void nikiforova::doComplement(const std::string& newDataset, const std::string& dataset1, const std::string& dataset2, DictOfDicts& dict)
{
  nikiforova::DictOfDicts::const_iterator cIter1 = dict.find(dataset1);
  nikiforova::DictOfDicts::const_iterator cIter2 = dict.find(dataset2);
  if ((cIter1 == dict.cend()) || cIter2 == dict.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict result = Dict(cIter1->second);
  auto listIter = cIter2->second.cbegin();
  while (listIter != cIter2->second.cend())
  {
    auto key = *listIter;
    if (result.find(key) != result.end())
    {
      result.erase(key);
    }
    listIter++;
  }
  if (dict.find(newDataset) != dict.end())
  {
    dict.erase(newDataset);
  }
  dict.insert(std::make_pair(newDataset, result));
}

void nikiforova::complement(std::string& str, DictOfDicts& dict)
{
  std::string newDataset = nikiforova::getWord(str);
  std::string dataset1 = nikiforova::getWord(str);
  std::string dataset2 = nikiforova::getWord(str);
  nikiforova::doComplement(newDataset, dataset1, dataset2, dict);
}

void nikiforova::doIntersect(const std::string& newDataset, const std::string& dataset1, const std::string& dataset2, DictOfDicts& dict)
{
  nikiforova::DictOfDicts::const_iterator cIter1 = dict.find(dataset1);
  nikiforova::DictOfDicts::const_iterator cIter2 = dict.find(dataset2);
  if ((cIter1 == dict.cend()) || cIter2 == dict.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict result;
  auto listIter1 = cIter1->second.cbegin();
  while (listIter1 != cIter1->second.cend())
  {
    auto key1 = *listIter1;
    auto listIter2 = cIter2->second.cbegin();
    while (listIter2 != cIter2->second.cend())
    {
      auto key2 = *listIter2;
      if ((key1 == key2) && (result.find(key1) == result.end()))
      {
        result.insert(*listIter1);
      }
      listIter2++;
    }
    listIter1++;
  }
  if (dict.find(newDataset) != dict.end())
  {
    dict.erase(newDataset);
  }
  dict.insert(std::make_pair(newDataset, result));
}

void nikiforova::intersect(std::string& str, DictOfDicts& dict)
{
  std::string newDataset = nikiforova::getWord(str);
  std::string dataset1 = nikiforova::getWord(str);
  std::string dataset2 = nikiforova::getWord(str);
  nikiforova::doIntersect(newDataset, dataset1, dataset2, dict);
}

void nikiforova::doUnion(const std::string& newDataset, const std::string& dataset1, const std::string& dataset2, DictOfDicts& dict)
{
  nikiforova::DictOfDicts::const_iterator cIter1 = dict.find(dataset1);
  nikiforova::DictOfDicts::const_iterator cIter2 = dict.find(dataset2);
  if ((cIter1 == dict.cend()) || cIter2 == dict.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict result = dict.find(dataset1)->second;
  auto listIter = cIter2->second.cbegin();
  while (listIter != cIter2->second.cend())
  {
    auto key = *listIter;
    if (result.find(key) == result.end())
    {
      result.insert(*listIter);
    }
    listIter++;
  }
  if (dict.find(newDataset) != dict.end())
  {
    dict.erase(newDataset);
  }
  dict.insert(std::make_pair(newDataset, result));
}

void nikiforova::myUnion(std::string& str, DictOfDicts& dict)
{
  std::string newDataset = nikiforova::getWord(str);
  std::string dataset1 = nikiforova::getWord(str);
  std::string dataset2 = nikiforova::getWord(str);
  nikiforova::doUnion(newDataset, dataset1, dataset2, dict);
}

std::ostream& nikiforova::getWordsWithPrefix(std::ostream& out, const std::string& prefix, const Dict& dict)
{
  if (dict.empty())
  {
    nikiforova::emptyMessage(out);
    out << "\n";
    return out;
  }
  std::set< std::string >::iterator iter = dict.begin();
  std::string temp = *iter;
  while (temp < prefix)
  {
    iter++;
    temp = *iter;
  }
  if (!nikiforova::hasPrefix(temp, prefix))
  {
    return out << "Words with prefix = " << prefix << " doesn't exist" << "\n";
  }
  bool flag = 1;
  while (temp >= prefix)
  {
    if (flag)
    {
      out << temp;
      flag = 0;
    }
    else
    {
      out << " " << temp;
    }
    iter++;
    if (iter == dict.end())
    {
      return out << "\n";
    }
    temp = *iter;
    if (!nikiforova::hasPrefix(temp, prefix))
    {
      return out << "\n";
    }
  }
  return out;
}

void nikiforova::wordsWithPrefix(std::string& str, const DictOfDicts& dicts)
{
  std::string nameOfDict = nikiforova::getWord(str);
  std::string prefix = nikiforova::getWord(str);
  nikiforova::DictOfDicts::const_iterator cIter = dicts.find(nameOfDict);
  if (cIter == dicts.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict dict = cIter->second;
  nikiforova::getWordsWithPrefix(std::cout, prefix, dict);
}

std::ostream& nikiforova::isContain(std::ostream& out, const std::string& word, const Dict& dict)
{
  std::set< std::string >::const_iterator iter = dict.find(word);
  return out << std::boolalpha << (iter != dict.cend()) << "\n";
}

void nikiforova::contain(std::string& str, const DictOfDicts& dicts)
{
  std::string nameOfDict = nikiforova::getWord(str);
  std::string word = nikiforova::getWord(str);
  nikiforova::DictOfDicts::const_iterator cIter = dicts.find(nameOfDict);
  if (cIter == dicts.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict dict = cIter->second;
  nikiforova::isContain(std::cout, word, dict);
}

void nikiforova::save(std::string& str, const DictOfDicts& dicts)
{
  std::string nameOfDict = nikiforova::getWord(str);
  std::string nameOfFile = nikiforova::getWord(str);
  nikiforova::DictOfDicts::const_iterator cIter = dicts.find(nameOfDict);
  if (cIter == dicts.cend())
  {
    nikiforova::invalidCommandMessage(std::cout);
    std::cout << "\n";
    return;
  }
  nikiforova::Dict dict = cIter->second;
  nikiforova::doSave(nameOfDict, nameOfFile, dict);
}

void nikiforova::doSave(const std::string& nameOfDict, const std::string& nameOfFile, const Dict& dict)
{
  std::fstream fOut(nameOfFile, std::ios::app);
  fOut << "\n";
  nikiforova::doPrint(fOut, nameOfDict, dict);
  fOut.close();
}

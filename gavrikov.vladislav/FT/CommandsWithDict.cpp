#include "CommandsWithDict.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "AuxiliaryMethods.hpp"
#include "ErrorMessages.hpp"

void gavrikov::createDict(dict_t& newDict, std::istream& in)
{
  std::string inputStr{};
  std::getline(in, inputStr);
  if (!inputStr.empty())
  {
    std::string inputName = getWord(inputStr);
    if (!isUniqueName(inputName, newDict))
    {
      nameRedefinitionMessage(std::cout);
      return;
    }
    gavrikov::enWords collection{};
    while (!inputStr.empty())
    {
      collection.insert(getWord(inputStr));
    }
    newDict.insert(std::make_pair(inputName, collection));
  }
}
void gavrikov::printDict(const dict_t& outDict, std::istream& in)
{
  std::string namePrintDict{};
  in >> namePrintDict;
  dict_t::const_iterator constIter = outDict.find(namePrintDict);
  if (constIter == outDict.cend())
  {
    errorNameMessage(std::cout);
    return;
  }
  gavrikov::enWords words = constIter->second;
  if (words.empty())
  {
    emptyCollectionMessage(std::cout);
    return;
  }
  std::cout << namePrintDict << ": ";
  std::copy(words.cbegin(), words.cend(), std::ostream_iterator< std::string >(std::cout, " "));
}
void gavrikov::insertWord(dict_t& nowDict, std::istream& in)
{
  std::string inputStr{};
  std::getline(in, inputStr);
  if (!inputStr.empty())
  {
    std::string nameDict = getWord(inputStr);
    if (isUniqueName(nameDict, nowDict))
    {
      errorNameMessage(std::cout);
      return;
    }
    dict_t::iterator iter = nowDict.find(nameDict);
    while (!inputStr.empty())
    {
      (iter->second).insert(getWord(inputStr));
    }
  }
}
void gavrikov::intersection(dict_t& ourDict, std::istream& in)
{
  std::string inputStr{};
  std::getline(in, inputStr);
  if (!inputStr.empty())
  {
    std::string firstOldDict = getWord(inputStr);
    if(isUniqueName(firstOldDict, ourDict))
    {
      errorNameMessage(std::cout);
      return;
    }
    std::string secondOldDict = getWord(inputStr);
    if (isUniqueName(secondOldDict, ourDict))
    {
      errorNameMessage(std::cout);
      return;
    }
    std::string newNameDict = getWord(inputStr);
    doInter(ourDict, firstOldDict, secondOldDict, newNameDict);
  }
}
void gavrikov::complement(dict_t& ourDict, std::istream& in)
{
  std::string inputStr{};
  std::getline(in, inputStr);
  if (!inputStr.empty())
  {
    std::string firstOldDict = getWord(inputStr);
    if (isUniqueName(firstOldDict, ourDict))
    {
      errorNameMessage(std::cout);
      return;
    }
    std::string secondOldDict = getWord(inputStr);
    if (isUniqueName(secondOldDict, ourDict))
    {
      errorNameMessage(std::cout);
      return;
    }
    std::string newNameDict = getWord(inputStr);
    doCompl(ourDict, firstOldDict, secondOldDict, newNameDict);
  }
}
void gavrikov::countPref(const dict_t& ourDict, std::istream& in)
{
  std::string nameDict{};
  in >> nameDict;
  if (isUniqueName(nameDict, ourDict))
  {
    errorNameMessage(std::cout);
    return;
  }
  dict_t::const_iterator dictIter = ourDict.find(nameDict);
  gavrikov::enWords collection = dictIter->second;
  std::string prefix{};
  in >> prefix;
  gavrikov::enWords::const_iterator collIter{};
  collIter = lower_bound(collection.begin(), collection.end(), prefix);
  std::size_t countPref = 0;
  if (collIter == collection.end())
  {
    std::cout << countPref;
    return;
  }
  std::string nowWord = *collIter;
  while (nowWord >= prefix)
  {
    if (!hasPrefix(nowWord, prefix))
    {
      break;
    }
    countPref++;
    collIter++;
    if (collIter == collection.end())
    {
      break;
    }
    nowWord = *collIter;
  }
  std::cout << countPref;
}
void gavrikov::dropDict(dict_t& ourDict, std::istream& in)
{
  std::string nameDict{};
  in >> nameDict;
  if (ourDict.find(nameDict) != ourDict.end())
  {
    gavrikov::dict_t::iterator clearIt = ourDict.find(nameDict);
    clearIt->second.clear();
  }
}
void gavrikov::unload(dict_t& ourDict, std::istream& in)
{
  std::string fileOutput{};
  in >> fileOutput;
  std::ifstream inFile(fileOutput);
  if (!inFile.is_open())
  {
    errorOpenFile(std::cout);
    return;
  }
  std::string newNameDict{};
  in >> newNameDict;
  if (!isUniqueName(newNameDict, ourDict))
  {
    nameRedefinitionMessage(std::cout);
    return;
  }
  gavrikov::enWords collection{};
  std::string inputStr{};
  while (getline(inFile, inputStr))
  {
    while (!inputStr.empty())
    {
      collection.insert(getWord(inputStr));
    }
  }
  ourDict.insert(std::make_pair(newNameDict, collection));
}
void gavrikov::load(dict_t& ourDict, std::istream& in)
{
  std::string fileInput{};
  in >> fileInput;
  std::ofstream outFile(fileInput);
  if (!outFile.is_open())
  {
    errorOpenFile(std::cout);
    return;
  }
  std::string oldNameDict{};
  in >> oldNameDict;
  if (isUniqueName(oldNameDict, ourDict))
  {
    errorNameMessage(std::cout);
    return;
  }
  dict_t::const_iterator constIter = ourDict.find(oldNameDict);
  auto cbegin = constIter->second.begin();
  auto cend = constIter->second.end();
  std::copy(cbegin, cend, std::ostream_iterator< std::string >(outFile, " "));
}

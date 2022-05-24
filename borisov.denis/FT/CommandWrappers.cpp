#include "CommandWrappers.h"
#include <fstream>

namespace
{
  borisov::Dict& getDict(borisov::AllDicts& allDicts, const std::string& dictName);
}

void borisov::addWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  Dict& dict = getDict(allDicts, *(argIter++));
  const Word& word = *(argIter++);
  const Translation& translation = *(argIter++);
  size_t count = std::stoull(*(argIter++));
  add(dict, word, translation, count);
}

void borisov::removeWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 2 && argList.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  Dict& dict = getDict(allDicts, *(argIter++));
  const Word& word = *(argIter++);
  if (argList.size() == 2)
  {
    remove(dict, word);
  }
  else
  {
    const Translation& translation = *(argIter++);
    remove(dict, word, translation);
  }
}

void borisov::manualWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  Dict& dict = getDict(allDicts, *(argIter++));
  const Word& word = *(argIter++);
  const Translation& translation = *(argIter++);
  long long count = std::stoll(*(argIter++));
  manual(dict, word, translation, count);
}

void borisov::wordsWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 1)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  words(out, getDict(allDicts, argList.front())) << '\n';
}

void borisov::infoWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  Dict& dict = getDict(allDicts, *(argIter++));
  const Word& word = *(argIter++);
  info(out, dict, word) << '\n';
}

void borisov::translateWrapper(std::ostream& out, AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  Dict& dict = getDict(allDicts, *(argIter++));
  const Word& word = *(argIter++);
  translate(out, dict, word) << '\n';
}

void borisov::saveWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  const std::string& name = *(argIter++);
  Dict& dict = getDict(allDicts, name);
  std::ofstream out(*(argIter++), std::ios_base::app);
  save(out, name, dict);
}

void borisov::uniteWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  const std::string& name = *(argIter++);
  bool s = allDicts.insert({name, Dict()}).second;
  if (!s)
  {
    throw std::invalid_argument("Invalid dict name");
  }
  Dict& result = getDict(allDicts, name);
  const Dict& dict1 = getDict(allDicts, *(argIter++));
  const Dict& dict2 = getDict(allDicts, *(argIter++));
  unite(result, dict1, dict2);
}

void borisov::intersectWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  const std::string& name = *(argIter++);
  bool s = allDicts.insert({name, Dict()}).second;
  if (!s)
  {
    throw std::invalid_argument("Invalid dict name");
  }
  Dict& result = getDict(allDicts, name);
  const Dict& dict1 = getDict(allDicts, *(argIter++));
  const Dict& dict2 = getDict(allDicts, *(argIter++));
  intersect(result, dict1, dict2);
}

void borisov::complementWrapper(AllDicts& allDicts, const ArgList& argList)
{
  if (argList.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto argIter = argList.cbegin();
  const std::string& name = *(argIter++);
  bool s = allDicts.insert({name, Dict()}).second;
  if (!s)
  {
    throw std::invalid_argument("Invalid dict name");
  }
  Dict& result = getDict(allDicts, name);
  const Dict& dict1 = getDict(allDicts, *(argIter++));
  const Dict& dict2 = getDict(allDicts, *(argIter++));
  complement(result, dict1, dict2);
}

namespace
{
  borisov::Dict& getDict(borisov::AllDicts& allDicts, const std::string& dictName)
  {
    auto dictIter = allDicts.find(dictName);
    if (dictIter == allDicts.end())
    {
      throw std::invalid_argument("Invalid dict name");
    }
    return dictIter->second;
  }
}

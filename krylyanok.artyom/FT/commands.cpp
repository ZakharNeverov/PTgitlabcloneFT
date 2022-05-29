#include "operations.hpp"
#include <iomanip>
#include "commands.hpp"

void krylyanok::Commands::getLoad()
{
  out_ << std::fixed << std::setprecision(1);
  std::string nameDict = "";
  std::string nameFile = "";
  in_ >> nameDict >> nameFile;
  if (!(findElem(nameDict) == -1))
  {
    throw std::logic_error("");
  }
  std::vector< std::string > words;
  makeDict(nameFile, words);
  dictionaries_.push_back({ words.size(), nameDict });
  for (std::string el : words)
  {
    dictionaries_[dictionaries_.size() - 1].add(el);
  }
  return;
}

void krylyanok::Commands::getPrint()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = 0;
  i = findElem(dictName);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  dictionaries_[i].printDictionary(out_);
  return;
}

void krylyanok::Commands::getTop()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  dictionaries_[i].printTop(out_);
  return;
}

void krylyanok::Commands::getSearch()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  std::string word = "";
  in_ >> word;
  dictionaries_[i].searhWord(out_, word);
  return;
}

void krylyanok::Commands::getMerge()
{
  bool succesfulMerge = 0;
  std::string tempName = "temp";
  std::string mergeDictName = "";
  std::string dictName1 = "";
  in_ >> mergeDictName >> dictName1;
  if (!(findElem(mergeDictName) == -1))
  {
    throw std::logic_error("");
  }
  size_t i1 = findElem(dictName1);
  if (i1 == -1)
  {
    throw std::logic_error("");
  }
  Dictionary mergeDicter(1, tempName);
  while (in_.peek() != '\n')
  {
    std::string dictName2 = "";
    in_ >> dictName2;
    size_t i2 = findElem(dictName2);
    if (i2 == -1)
    {
      throw std::logic_error("");
    }
    if (!succesfulMerge)
    {
      mergeDicter = std::move(mergeDict(dictionaries_[i1], dictionaries_[i2], mergeDictName));
    }
    else
    {
      mergeDicter = std::move(mergeDict(mergeDicter, dictionaries_[i2], mergeDictName));
    }
  }
  dictionaries_.push_back(std::move(mergeDicter));
  return;
}

void krylyanok::Commands::getDelete()
{
  std::string dictName = "";
  in_ >> dictName;
  size_t i = findElem(dictName);
  if (i == -1)
  {
    throw std::logic_error("");
  }
  dictionaries_.erase(dictionaries_.begin() + i);
  return;
}

size_t krylyanok::Commands::findElem(std::string& dictName)
{
  size_t i = 0;
  for (i; i < dictionaries_.size(); i++)
  {
    if (dictionaries_[i].getName() == dictName)
    {
      break;
    }
    if (dictionaries_.size() - 1 == i)
    {
      return -1;
    }
  }
  return i;
}

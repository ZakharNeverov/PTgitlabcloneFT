#include "WrappersForCommands.hpp"
#include "Commands.hpp"
#include "Dictionary.hpp"
#include "Utilites.hpp"

void roletskaya::insert(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::string fileName = getElem(line);
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  if (dict->second.insert(line) == true)
  {
    out << "Word is inserted.\n";
  }
}

void roletskaya::findWordByTranslation(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::string fileName = getElem(line);
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  if (dict->second.findWordByTranslation(line, out) == false)
  {
    out << "No such word in the dictionary.\n";
  }
}

void roletskaya::findTranslation(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::string fileName = getElem(line);
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  if (dict->second.findTranslation(line, out) == false)
  {
    out << "No such word in the dictionary.\n";
  }
}

void roletskaya::deleteKey(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::string fileName = getElem(line);
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  if (dict->second.deleteKey(line) == true)
  {
    out << "Word is deleted.\n";
  }
}

void roletskaya::print(std::string& fileName, dictsArray& dictsArray, std::ofstream& out)
{
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  dict->second.print(out);
}

void roletskaya::translateText(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::string fileName = getElem(line);
  if (isCorrectDictName(fileName, dictsArray) == false)
  {
    throw std::invalid_argument("Incorrect dictionary name.\n");
  }
  auto dict = dictsArray.find(fileName);
  dict->second.translateText(line, out);
}

void roletskaya::doMerge(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::vector< std::string > names;
  while (!line.empty())
  {
    names.push_back(getElem(line));
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Too few files.\n");
  }
  std::vector< Dictionary > dicts;
  for (size_t i = 0; i < names.size(); i++)
  {
    if (isCorrectDictName(names.at(i), dictsArray) == false)
    {
      throw std::invalid_argument("Incorrect dictionary name.\n");
    }
    auto item = dictsArray.find(names.at(i));
    dicts.push_back(item->second);
  }
  Dictionary newDict;
  newDict = merge(dicts);
  newDict.print(out);
}

void roletskaya::doComplement(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::vector< std::string > names;
  while (!line.empty())
  {
    names.push_back(getElem(line));
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Too few files.\n");
  }
  std::vector< Dictionary > dicts;
  for (size_t i = 0; i < names.size(); i++)
  {
    if (isCorrectDictName(names.at(i), dictsArray) == false)
    {
      throw std::invalid_argument("Incorrect dictionary name.\n");
    }
    auto item = dictsArray.find(names.at(i));
    dicts.push_back(item->second);
  }
  complement(dicts);
  dicts[0].print(out);
}

void roletskaya::ifEquals(std::string& line, dictsArray& dictsArray, std::ofstream& out)
{
  std::vector< std::string > names;
  while (!line.empty())
  {
    names.push_back(getElem(line));
  }
  if (names.size() < 2)
  {
    throw std::invalid_argument("Too few files.\n");
  }
  std::vector< Dictionary > dicts;
  for (size_t i = 0; i < names.size(); i++)
  {
    if (isCorrectDictName(names.at(i), dictsArray) == false)
    {
      throw std::invalid_argument("Incorrect dictionary name.\n");
    }
    auto item = dictsArray.find(names.at(i));
    dicts.push_back(item->second);
  }
  if (equals(dicts) == true)
  {
    out << "Dictionaries are equal.\n";
  }
  else
  {
    out << "Dictionaries are not equal.\n";
  }
}

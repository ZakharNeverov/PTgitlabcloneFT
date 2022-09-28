#include "Dictionary-interaction.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <functional>
#include "Words-commands.hpp"

void akimov::isAvailable(const vectorD& dicts, cnstStr& name)
{
  auto check = std::bind(isName(), std::placeholders::_1, name);
  auto iter = std::find_if(dicts.begin(), dicts.end(), check);
  if (iter != dicts.end())
  {
    throw std::invalid_argument("Dictionary with this name already exists.");
  }
}
auto akimov::isFound(vectorD& dicts, cnstStr& name)
{
  auto check = std::bind(isName(), std::placeholders::_1, name);
  auto iter = std::find_if(dicts.begin(), dicts.end(), check);
  if (iter == dicts.end())
  {
    throw std::invalid_argument("Dictionary " + name + " wasn't found.");
  }
  return iter;
}
auto akimov::isFound(const vectorD& dicts, cnstStr& name)
{
  auto check = std::bind(isName(), std::placeholders::_1, name);
  auto iter = std::find_if(dicts.begin(), dicts.end(), check);
  if (iter == dicts.end())
  {
    throw std::invalid_argument("Dictionary " + name + " wasn't found.");
  }
  return iter;
}
void akimov::getNamesAndDoCommand(std::istream& input, vectorD& dicts, cnstStr& cmd)
{
  std::string name{}, dict1{}, dict2{};
  input >> name >> dict1 >> dict2;
  char symb{};
  if (cmd == "intersectionFromLetter")
  {
    input >> symb;
  }
  isAvailable(dicts, name);
  auto dictionary1 = isFound(dicts, dict1);
  auto dictionary2 = isFound(dicts, dict2);
  if (cmd == "add" || cmd == "remove")
  {
    akimov::Dictionary newDictionary(*dictionary1);
    newDictionary.rename(name);
    if (cmd == "add")
    {
      for (auto& i: *dictionary2)
      {
        newDictionary.insertPair(i);
      }
    }
    else if (cmd == "remove")
    {
      for (auto& i: *dictionary2)
      {
        newDictionary.subtractPair(i);
      }
    }
    dicts.push_back(newDictionary);
  }
  else if (cmd == "intersection" || cmd == "intersectionFromLetter")
  {
    intersectionCommand(dicts, name, dictionary1, dictionary2, cmd, symb);
  }
}
void akimov::intersectionCommand(vectorD& dicts, cnstStr& name, vectorCIt it1, vectorCIt it2, cnstStr& cmd, char c)
{
  akimov::Dictionary newDictionary(name);
  for (auto& i: *it1)
  {
    if (cmd == "intersectionFromLetter" && i.first[0] != c)
    {
      continue;
    }
    auto item = it2->search(i.first);
    if (item == it2->end())
    {
      continue;
    }
    if (i.second <= (*item).second)
    {
      newDictionary.insertPair(i);
    }
    else
    {
      newDictionary.insertPair(*item);
    }
  }
  dicts.push_back(newDictionary);
}

void akimov::create(std::istream& input, vectorD& dicts)
{
  std::string name{};
  input >> name;
  isAvailable(dicts, name);
  akimov::Dictionary dict(name);
  std::string text{};
  std::getline(input, text);
  std::string word = getWord(text);
  while (word != "<NONE>")
  {
    dict.insert(word);
    word = getWord(text); 
  }
  dicts.push_back(dict);
}
void akimov::print(std::istream& input, std::ostream& output, const vectorD& dicts)
{
  std::string name{};
  input >> name;
  auto iter = isFound(dicts, name);
  iter->print(output);
  output << '\n';
}
void akimov::getMostPopular(std::istream& input, std::ostream& output, const vectorD& dicts)
{
  std::string name{};
  input >> name;
  auto dict = isFound(dicts, name);
  dict->printMostPopular(output);
  output<< '\n';
}
void akimov::drop(std::istream& input, vectorD& dicts)
{
  std::string name{};
  input >> name;
  auto dict = isFound(dicts, name);
  dict->drop();
}
void akimov::find(std::istream& input, std::ostream& output, const vectorD& dicts)
{
  std::string name{}, word{};
  input >> name >> word;
  auto iter = isFound(dicts, name);
  output << int(iter->search(word) != iter->end());
  output << '\n';
}
void akimov::add(std::istream& input, vectorD& dicts)
{
  getNamesAndDoCommand(input, dicts, "add");
}
void akimov::remove(std::istream& input, vectorD& dicts)
{
  getNamesAndDoCommand(input, dicts, "remove");
}
void akimov::intersection(std::istream& input, vectorD& dicts)
{
  getNamesAndDoCommand(input, dicts, "intersection");
}
void akimov::intersectionFromLetter(std::istream& input, vectorD& dicts)
{
  getNamesAndDoCommand(input, dicts, "intersectionFromLetter");
}

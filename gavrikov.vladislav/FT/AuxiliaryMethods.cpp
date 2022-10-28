#include "AuxiliaryMethods.hpp"
#include <iostream>
#include "ErrorMessages.hpp"

void gavrikov::doInter(dict_t& dict, cRefStr first, cRefStr second, cRefStr newName)
{
  if (!isUniqueName(newName, dict))
  {
    nameRedefinitionMessage(std::cout);
    return;
  }
  dict_t::const_iterator constIter1 = dict.find(first);
  dict_t::const_iterator constIter2 = dict.find(second);
  if ((constIter1 == dict.cend()) || constIter2 == dict.cend())
  {
    errorNameMessage(std::cout);
    return;
  }
  gavrikov::enWords collection{};
  doInterCycle(constIter1, constIter2, collection);
  dict.insert(std::make_pair(newName, collection));
}
void gavrikov::doCompl(dict_t& dict, cRefStr first, cRefStr second, cRefStr& newName)
{
  if (!isUniqueName(newName, dict))
  {
    nameRedefinitionMessage(std::cout);
    return;
  }
  dict_t::const_iterator constIter1 = dict.find(first);
  dict_t::const_iterator constIter2 = dict.find(second);
  if ((constIter1 == dict.cend()) || constIter2 == dict.cend())
  {
    errorNameMessage(std::cout);
    return;
  }
  gavrikov::enWords collection{};
  doComplCycle(constIter1, constIter2, collection);
  dict.insert(std::make_pair(newName, collection));
}
void gavrikov::doInterCycle(cIterDict& iter1, cIterDict& iter2, enWords& res)
{
  auto pointer1 = iter1->second.cbegin();
  while (pointer1 != iter1->second.cend())
  {
    auto pointer2 = iter2->second.cbegin();
    bool isUnique = false;
    while (pointer2 != iter2->second.cend())
    {
      if (*pointer1 == *pointer2)
      {
        isUnique = true;
        break;
      }
      pointer2++;
    }
    if (isUnique)
    {
      res.insert(*pointer1);
    }
    pointer1++;
  }
}
void gavrikov::doComplCycle(cIterDict& iter1, cIterDict& iter2, enWords& res)
{
  doCycle(iter1, iter2, res);
  doCycle(iter2, iter1, res);
}
void gavrikov::doCycle(cIterDict& iter1, cIterDict& iter2, enWords& res)
{
  auto pointer1 = iter1->second.cbegin();
  while (pointer1 != iter1->second.cend())
  {
    auto pointer2 = iter2->second.cbegin();
    bool isUnique = false;
    while (pointer2 != iter2->second.cend())
    {
      if (*pointer1 == *pointer2)
      {
        isUnique = true;
        break;
      }
      pointer2++;
    }
    if (!isUnique)
    {
      res.insert(*pointer1);
    }
    pointer1++;
  }
}
std::string gavrikov::getWord(std::string& inputStr)
{
  std::string result{};
  while (inputStr[0] == ' ')
  {
    inputStr.erase(0, 1);
  }
  result = inputStr.substr(0, inputStr.find(" "));
  inputStr = inputStr.erase(0, inputStr.find(" "));
  return result;
}
bool gavrikov::hasPrefix(cRefStr inputStr, cRefStr prefix)
{
  for (std::size_t i = 0; i < inputStr.length(); ++i)
  {
    if (i >= prefix.length())
    {
      break;
    }
    if (inputStr[i] != prefix[i])
    {
      return false;
    }
  }
  return true;
}
bool gavrikov::isUniqueName(cRefStr str, const dict_t& name)
{
  dict_t::const_iterator constIter = name.find(str);
  if (constIter == name.cend())
  {
    return true;
  }
  return false;
}

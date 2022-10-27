#include "AuxiliaryMethods.hpp"
#include <iostream>
#include "ErrorMessages.hpp"

void gavrikov::doInter(dict_t& dict, constStr& first, constStr& second, constStr& newName)
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
  bool isUnique = true;
  doCycle(constIter1, constIter2, collection, isUnique);
  dict.insert(std::make_pair(newName, collection));
}
void gavrikov::doCompl(dict_t& dict, constStr& first, constStr& second, constStr& newName)
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
  bool isUnique = false;
  doCycle(constIter1, constIter2, collection, isUnique);
  doCycle(constIter2, constIter1, collection, isUnique);
  dict.insert(std::make_pair(newName, collection));
}
void gavrikov::doCycle(cIterDict& iter1, cIterDict& iter2, enWords& res, bool flag)
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
    if (isUnique == flag)
    {
      res.insert(*pointer1);
    }
    pointer1++;
  }
}
std::string gavrikov::getWord(std::string& inputStr)
{
  std::string result{};
  if (inputStr[0] == ' ')
  {
    inputStr.erase(0, 1);
  }
  result = inputStr.substr(0, inputStr.find(" "));
  inputStr = inputStr.erase(0, inputStr.find(" "));
  return result;
}
bool gavrikov::hasPrefix(constStr& inputStr, constStr& prefix)
{
  for (size_t i = 0; i < inputStr.length(); ++i)
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
bool gavrikov::isUniqueName(constStr& str, const dict_t& name)
{
  dict_t::const_iterator constIter = name.find(str);
  if (constIter == name.cend())
  {
    return true;
  }
  return false;
}

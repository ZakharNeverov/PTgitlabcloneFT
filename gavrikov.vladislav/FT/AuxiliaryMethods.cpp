#include "AuxiliaryMethods.hpp"
#include <iostream>
#include "ErrorMessages.hpp"

void gavrikov::doInter(dict_t& dict, std::string first, std::string second, std::string newName)
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
  auto pointer1 = constIter1->second.cbegin();
  while (pointer1 != constIter1->second.cend())
  {
    auto pointer2 = constIter2->second.cbegin();
    bool isUnique = false;
    while (pointer2 != constIter2->second.cend())
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
      collection.insert(*pointer1);
    }
    pointer1++;
  }
  dict.insert(std::make_pair(newName, collection));
}
void gavrikov::doCompl(dict_t& dict, std::string first, std::string second, std::string newName)
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
  gavrikov::enWords result;
  auto pointer1 = constIter1->second.cbegin();
  while (pointer1 != constIter1->second.cend())
  {
    auto pointer2 = constIter2->second.cbegin();
    bool isUnique = false;
    while (pointer2 != constIter2->second.cend())
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
      result.insert(*pointer1);
    }
    pointer1++;
  }
  auto pointer2 = constIter2->second.cbegin();
  while (pointer2 != constIter2->second.cend())
  {
    auto pointer1 = constIter1->second.cbegin();
    bool isUnique = false;
    while (pointer1 != constIter1->second.cend())
    {
      if (*pointer1 == *pointer2)
      {
        isUnique = true;
        break;
      }
      pointer1++;
    }
    if (!isUnique)
    {
      result.insert(*pointer2);
    }
    pointer2++;
  }
  dict.insert(std::make_pair(newName, result));
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
bool gavrikov::hasPrefix(const std::string& inputStr, const std::string& prefix)
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
bool gavrikov::isUniqueName(std::string str, const dict_t& name)
{
  dict_t::const_iterator constIter = name.find(str);
  if (constIter == name.cend())
  {
    return true;
  }
  return false;
}

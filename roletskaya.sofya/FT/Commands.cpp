#include "Commands.hpp"
#include "Utilites.hpp"

bool roletskaya::isCorrectDictName(std::string& name, dictsArray& dictsArray)
{
  return dictsArray.find(name) != dictsArray.end();
}

bool roletskaya::pushDictToArray(std::string& name, std::string fileName, dictsArray& dictsArray)
{
  std::ifstream fin;
  fin.open((fileName));
  roletskaya::Dictionary dict;
  if (dict.createDictionary(fin) == true)
  {
    dictsArray.emplace(name, dict);
    fin.close();
    return true;
  }
  fin.close();
  return false;
}

map roletskaya::merge(std::vector< Dictionary >& dictsVector)
{
  map newDictionary = dictsVector[0].dictionary_;
  for (size_t i = 1; i < dictsVector.size(); i++)
  {
    if (dictsVector[i].dictionary_.empty())
    {
      continue;
    }
    for (auto iter = dictsVector[i].dictionary_.begin(); iter != dictsVector[i].dictionary_.end(); iter++)
    {
      auto item = newDictionary.find(iter->first);
      if (item == newDictionary.end())
      {
        auto item2 = dictsVector[i].dictionary_.find(iter->first);
        newDictionary.emplace(item2->first, item2->second);
      }
      else
      {
        continue;
      }
    }
  }
  return newDictionary;
}

void roletskaya::complement(std::vector< Dictionary >& dictsVector)
{
  for (size_t i = 1; i < dictsVector.size(); i++)
  {
    if (dictsVector[i].dictionary_.empty())
    {
      continue;
    }
    for (auto iter = dictsVector[i].dictionary_.begin(); iter != dictsVector[i].dictionary_.end(); iter++)
    {
      auto item = dictsVector[0].dictionary_.find(iter->first);
      if (item == dictsVector[0].dictionary_.end())
      {
        continue;
      }
      else
      {
        if (iter->second == item->second)
        {
          dictsVector[0].dictionary_.erase(iter->first);
        }
      }
    }
  }
}

bool roletskaya::equals(std::vector< Dictionary >& dictsVector)
{
  int count = 0;
  for (size_t i = 0; i < dictsVector.size() - 1; i++)
  {
    if (dictsVector[i].dictionary_.size() != dictsVector[i + 1].dictionary_.size())
    {
      return false;
    }
    for (auto iter = dictsVector[i + 1].dictionary_.begin(); iter != dictsVector[i + 1].dictionary_.end(); iter++)
    {
      auto item = dictsVector[i].dictionary_.find(iter->first);
      if (item == dictsVector[i].dictionary_.end())
      {
        return false;
      }
      else
      {
        count++;
        if (item->second != iter->second)
        {
          return false;
        }
      }
    }
    if (count != dictsVector[i].dictionary_.size())
    {
      return false;
    }
    count = 0;
  }
  return true;
}

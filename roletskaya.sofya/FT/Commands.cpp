#include "Commands.hpp"
#include "Utilites.hpp"

std::vector< map > roletskaya::getFiles(std::string& files)
{
  std::vector< map > vector;
  std::string fileName = "";
  std::ifstream in;
  while (!files.empty())
  {
    Dictionary dictionary;
    fileName = getElem(files);
    in.open(fileName);
    if (!in)
    {
      throw std::invalid_argument("Couldn't open the file.\n");
    }
    dictionary.createDictionary(in);
    in.close();
    vector.push_back(dictionary.dictionary_);
  }
  return vector;
}

map roletskaya::merge(std::vector< map >& dictsVector)
{
  map newDictionary = std::move(dictsVector[0]);
  for (int i = 1; i < dictsVector.size(); i++)
  {
    if (dictsVector[i].size() == 0)
    {
      continue;
    }
    for (auto iter = dictsVector[i].begin(); iter != dictsVector[i].end(); iter++)
    {
      auto item = newDictionary.find(iter->first);
      if (item == newDictionary.end())
      {
        auto item2 = dictsVector[i].find(iter->first);
        newDictionary.emplace(item2->first, item2->second);
      } else
      {
        continue;
      }
    }
  }
  return newDictionary;
}

map roletskaya::complement(std::vector< map >& dictsVector)
{
  for (int i = 1; i < dictsVector.size(); i++)
  {
    if (dictsVector[i].size() == 0)
    {
      continue;
    }
    for (auto iter = dictsVector[i].begin(); iter != dictsVector[i].end(); iter++)
    {
      auto item = dictsVector[0].find(iter->first);
      if (item == dictsVector[0].end())
      {
        continue;
      } else
      {
        if (iter->second.size() == item->second.size())
        {
          dictsVector[0].erase(iter->first);
        } else
        {
          for (auto listIter = iter->second.begin(); listIter != iter->second.end(); listIter++)
          {
            for (auto listIter2 = item->second.begin(); listIter2 != item->second.end(); listIter2++)
            {
              if (*listIter == *listIter2)
              {
                listIter2->erase();
              }
            }
          }
        }
      }
    }
  }
  return dictsVector[0];
}

bool roletskaya::equals(std::vector< map >& dictsVector)
{
  int countList = 0;
  int count = 0;
  bool flag = false;
  for (int i = 0; i < dictsVector.size() - 1; i++)
  {
    if (dictsVector[i].size() != dictsVector[i + 1].size())
    {
      return false;
    }
    for (auto iter = dictsVector[i].begin(); iter != dictsVector[i].end(); iter++)
    {
      for (auto iter2 = dictsVector[i + 1].begin(); iter2 != dictsVector[i + 1].end(); iter2++)
      {
        if (iter->first == iter2->first)
        {
          count++;
          if (iter->second.size() != iter2->second.size())
          {
            return false;
          }
          for (auto listIter = iter->second.begin(); listIter != iter->second.end(); listIter++)
          {
            for (auto listIter2 = iter2->second.begin(); listIter2 != iter2->second.end(); listIter2++)
            {
              if (*listIter == *listIter2)
              {
                countList++;
                break;
              }
            }
          }
          if (countList != iter->second.size())
          {
            return false;
          } else
          {
            countList = 0;
            break;
          }
        }
      }
    }
    if (count != dictsVector[i].size())
    {
      return false;
    } else
    {
      count = 0;
      flag = true;
    }
  }
  return flag;
}

bool roletskaya::checkResults(std::string& outFileName, std::string& ResultsFileName)
{
  std::ifstream in;
  std::ifstream checkIn;
  in.open(outFileName);
  checkIn.open(ResultsFileName);
  if (!checkIn)
  {
    throw std::invalid_argument("Couldn't open the result file.\n");
  }
  std::string line1 = "";
  std::string line2 = "";
  while (!in.eof() || !checkIn.eof())
  {
    std::getline(in, line1);
    std::getline(checkIn, line2);
    if (line1 != line2)
    {
      in.close();
      checkIn.close();
      return false;
    }
  }
  if ((in.eof() && !checkIn.eof()) || (!in.eof() && checkIn.eof()))
  {
    in.close();
    checkIn.close();
    return false;
  }
  in.close();
  checkIn.close();
  return true;
}

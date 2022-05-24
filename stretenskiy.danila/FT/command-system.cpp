#include "command-system.hpp"

#include <iostream>
#include <algorithm>
#include "ioKeys.hpp"

namespace
{
  size_t getIndexDict(const stretenskiy::function::nameDict &, const std::string &);
  size_t findNameDict(std::istream &, const stretenskiy::function::nameDict &);
}

namespace stretenskiy
{
  void function::creationDict(std::ostream &out, vecDict &vecDict, nameDict &vecName, std::istream &in)
  {
    std::string nameD;
    in >> nameD;
    if (std::find(vecName.begin(), vecName.end(), nameD) != vecName.end())
    {
      throw std::logic_error("This dict has already");
    }
    vecName.push_back(nameD);
    myDict dict;
    vecDict.push_back(dict);
  }

  void function::add(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    std::string word;
    in >> word;
    if (!word.empty())
    {
      while (checkContinueInputWord(in))
      {
        std::string transl;
        in >> transl;
        vecDict[index][word].insert(transl);
      }
      return;
    }
    throw std::logic_error("Enter the word");
  }

  void function::removeWord(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    std::string word;
    in >> word;
    if (!word.empty())
    {
      if (checkContinueInputWord(in))
      {
        std::string transl;
        in >> transl;
        if (!transl.empty())
        {
          do
          {
            vecDict[index][word].erase(transl);
          }
          while (checkContinueInputWord(in) && in >> transl);
          return;
        }
      }
      vecDict[index].erase(word);
      return;
    }
    throw std::logic_error("Enter the word");
  }

  void function::search(std::ostream &out, const vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    std::string word;
    in >> word;
    if (!word.empty())
    {
      auto iter = vecDict[index].find(word);
      if (iter != vecDict[index].end())
      {
        std::string transl;
        if (checkContinueInputWord(in) && in >> transl)
        {
          if (iter->second.find(transl) != iter->second.end())
          {
            out << "Translate " << transl << " success find\n";
          }
          else
          {
            out << "Translate " << transl << " don't find\n";
          }
          return;
        }
        out << "Word " << word << " success find and this his translate:";
        for (const auto &i : iter->first)
        {
          std::cout << ' ' << i;
        }
        return;
      }
      else
      {
        throw std::logic_error("The right word was not found in the dictionary");
      }
    }
    throw std::logic_error("Enter the word");
  }

  void function::clearDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    while (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      if (vecDict[index].find(word) != vecDict[index].end())
      {
        vecDict[index][word].clear();
      }
      else
      {
        out << "The right word was not found in the dictionary\n";
      }
    }
    vecDict[index].clear();
  }

  void function::unionDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t indexBase = findNameDict(in, vecName);
    size_t indexOne = findNameDict(in, vecName);
    function::myDict dictBase = vecDict[indexOne];
    int count = 1;
    while (checkContinueInputWord(in))
    {
      std::string nameTwo;
      in >> nameTwo;
      auto iter = std::find(vecName.begin(), vecName.end(), nameTwo);
      if (iter == vecName.end())
      {
        out << "Name " << nameTwo << " don't find in dict, create him\n";
        continue;
      }
      size_t indexTwo = iter - vecName.begin();
      auto dict_it = vecDict[indexTwo].begin();
      while (dict_it != vecDict[indexTwo].end())
      {
        dictBase[dict_it->first].insert(dict_it->second.begin(), dict_it->second.end());
        dict_it++;
      }
      count++;
    }
    if (count > 1)
    {
      vecDict[indexBase] = dictBase;
    }
    else
    {
      out << "Insufficient data\n";
    }
  }

  void function::intersectDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t indexBase = findNameDict(in, vecName);
    size_t indexOne = findNameDict(in, vecName);
    function::myDict dictBase = vecDict[indexOne];
    int count = 1;
    while (checkContinueInputWord(in))
    {
      std::string nameTwo;
      in >> nameTwo;
      auto iter = std::find(vecName.begin(), vecName.end(), nameTwo);
      if (iter == vecName.end())
      {
        out << "Name " << nameTwo << " don't find in dict, create him\n";
        continue;
      }
      size_t indexTwo = iter - vecName.begin();
      auto dict_it = dictBase.begin();
      while (dict_it != dictBase.end())
      {
        auto set_two = vecDict[indexTwo].find(dict_it->first);
        if (set_two != vecDict[indexTwo].end())
        {
          std::set< std::string > setTemp;
          auto iBegOne = dict_it->second.begin();
          auto iEndOne = dict_it->second.end();
          auto iBegTwo = set_two->second.begin();
          auto iEndTwo = set_two->second.end();
          std::set_intersection(iBegOne, iEndOne, iBegTwo, iEndTwo, std::inserter(setTemp, setTemp.begin()));
          dictBase[dict_it->first] = setTemp;
        }
        else
        {
          auto temp_it = dict_it->first;
          dict_it++;
          dictBase.erase(temp_it);
          continue;
        }
        dict_it++;
      }
      count++;
    }
    if (count > 1)
    {
      vecDict[indexBase] = dictBase;
    }
    else
    {
      out << "Insufficient data\n";
    }
  }

  void function::print(std::ostream &out, const vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    while (checkContinueInputWord(in))
    {
      size_t index = findNameDict(in, vecName);
      out << "DICTIONARY " << vecName[index] << '\n';
      out << vecDict[index];
    }
  }
}

namespace
{
  size_t getIndexDict(const stretenskiy::function::nameDict &vecName, const std::string &nameD)
  {
    auto iter = std::find(vecName.begin(), vecName.end(), nameD);
    if (iter == vecName.end())
    {
      throw std::logic_error("This name don't find in dict, create him");
    }
    return iter - vecName.begin();
  }

  size_t findNameDict(std::istream &in, const stretenskiy::function::nameDict &vecName)
  {
    std::string nameD;
    in >> nameD;
    size_t index = getIndexDict(vecName, nameD);
    return index;
  }
}

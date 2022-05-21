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
      std::string transl;
      while (checkContinueInputWord(in) && in >> transl)
      {
        vecDict[index][word].insert(transl);
      }
      return;
    }
    throw std::invalid_argument("Enter the name of the dictionary\n");
  }

  void function::removeWord(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    std::string word;
    in >> word;
    if (!word.empty())
    {
      std::string transl;
      if (checkContinueInputWord(in) && in >> transl)
      {
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
    throw std::invalid_argument("Enter the name of the dictionary\n");
  }

  void function::search(std::ostream &out, const vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    std::string word;
    in >> word;
    if (!word.empty())
    {
      std::string transl;
      if (checkContinueInputWord(in) && in >> transl)
      {
        if (vecDict[index].at(word).find(transl) != vecDict[index].at(word).end())
        {
          out << "Translate " << transl << " success find\n";
        }
        else
        {
          out << "Translate " << transl << " don't find\n";
        }
        return;
      }
      vecDict[index].at(word);
      out << "Word " << word << " success find and this his translate:";
      for (const auto &i : vecDict[index].at(word))
      {
        std::cout << ' ' << i;
      }
      return;
    }
    throw std::invalid_argument("Enter the name of the dictionary\n");
  }

  void function::clearDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t index = findNameDict(in, vecName);
    if (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      vecDict.at(index).at(word).clear();
      return;
    }
    vecDict.at(index).clear();
  }

  void function::unionDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t indexBase = findNameDict(in, vecName);
    size_t indexOne = findNameDict(in, vecName);
    function::myDict dictBase = vecDict[indexOne];
    do
    {
      size_t indexTwo = findNameDict(in, vecName);
      auto dict_it = vecDict[indexTwo].begin();
      while (dict_it != vecDict[indexTwo].end())
      {
        dictBase[dict_it->first].insert(dict_it->second.begin(), dict_it->second.end());
        dict_it++;
      }
    }
    while (checkContinueInputWord(in));
    vecDict[indexBase] = dictBase;
  }

  void function::intersectDict(std::ostream &out, vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    size_t indexBase = findNameDict(in, vecName);
    size_t indexOne = findNameDict(in, vecName);
    function::myDict dictBase = vecDict[indexOne];
    do
    {
      size_t indexTwo = findNameDict(in, vecName);
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
    }
    while (checkContinueInputWord(in));
    vecDict[indexBase] = dictBase;
  }

  void function::print(std::ostream &out, const vecDict &vecDict, const nameDict &vecName, std::istream &in)
  {
    while (checkContinueInputWord(in))
    {
      size_t index = findNameDict(in, vecName);
      out << "Dictionary " << vecName[index] << '\n';
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

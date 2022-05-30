#include "command-system.hpp"

#include <iostream>
#include <algorithm>
#include <functional>
#include "ioProccessing.hpp"
#include "warningMessages.hpp"

namespace
{
  bool isEqualName(const stretenskiy::Dictionary &, const std::string &);
  size_t getIndexDict(std::ostream &, const std::string &, const stretenskiy::function::vecDict &);
  size_t findNameDict(std::ostream &, std::istream &, const stretenskiy::function::vecDict &);
}

namespace stretenskiy
{
  void function::creationDict(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    std::string nameD;
    in >> nameD;
    using namespace std::placeholders;
    auto iter = std::find_if(vecDict.begin(), vecDict.end(), std::bind(isEqualName, _1, nameD));
    if (iter != vecDict.end())
    {
      informExistDict(out);
      return;
    }
    Dictionary dict{nameD};
    vecDict.push_back(dict);
  }

  void function::add(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    size_t index = findNameDict(out, in, vecDict);
    if (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      if (!word.empty())
      {
        while (checkContinueInputWord(in))
        {
          std::string transl;
          in >> transl;
          vecDict[index].content_[word].insert(transl);
        }
        return;
      }
    }
    informEnterWord(out);
  }

  void function::removeWord(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    size_t index = findNameDict(out, in, vecDict);
    if (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      if (!word.empty())
      {
        if (vecDict[index].content_.find(word) == vecDict[index].content_.end())
        {
          informNotFindWord(out);
          return;
        }
        if (checkContinueInputWord(in))
        {
          std::string transl;
          in >> transl;
          if (!transl.empty())
          {
            do
            {
              vecDict[index].content_[word].erase(transl);
            }
            while (checkContinueInputWord(in) && in >> transl);
            return;
          }
        }
        vecDict[index].content_.erase(word);
        return;
      }
    }
    informEnterWord(out);
  }

  void function::search(std::ostream &out, const vecDict &vecDict, std::istream &in)
  {
    size_t index = findNameDict(out, in, vecDict);
    if (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      if (!word.empty())
      {
        auto iter = vecDict[index].content_.find(word);
        if (iter != vecDict[index].content_.end())
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
          out << "Word " << word << " success find\n";
          return;
        }
        else
        {
          informNotFindWord(out);
          return;
        }
      }
    }
    informEnterWord(out);
  }

  void function::clearDict(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    size_t index = findNameDict(out, in, vecDict);
    while (checkContinueInputWord(in))
    {
      std::string word;
      in >> word;
      if (vecDict[index].content_.find(word) != vecDict[index].content_.end())
      {
        vecDict[index].content_[word].clear();
      }
      else
      {
        informNotFindWord(out);
        return;
      }
    }
    vecDict[index].content_.clear();
  }

  void function::unionDict(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    size_t indexBase = findNameDict(out, in, vecDict);
    size_t indexOne = findNameDict(out, in, vecDict);
    Dictionary dictBase = vecDict[indexOne];
    dictBase.name_ = vecDict[indexBase].name_;
    int count = 1;
    while (checkContinueInputWord(in))
    {
      using namespace std::placeholders;
      size_t indexTwo = findNameDict(out, in, vecDict);
      if (indexTwo == vecDict.size())
      {
        continue;
      }
      auto dict_it = vecDict[indexTwo].content_.begin();
      while (dict_it != vecDict[indexTwo].content_.end())
      {
        dictBase.content_[dict_it->first].insert(dict_it->second.begin(), dict_it->second.end());
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
      informInsufficientData(out);
    }
  }

  void function::intersectDict(std::ostream &out, vecDict &vecDict, std::istream &in)
  {
    size_t indexBase = findNameDict(out, in, vecDict);
    size_t indexOne = findNameDict(out, in, vecDict);
    Dictionary dictBase = vecDict[indexOne];
    dictBase.name_ = vecDict[indexBase].name_;
    int count = 1;
    while (checkContinueInputWord(in))
    {
      using namespace std::placeholders;
      size_t indexTwo = findNameDict(out, in, vecDict);
      if (indexTwo == vecDict.size())
      {
        continue;
      }
      auto dict_it = dictBase.content_.begin();
      while (dict_it != dictBase.content_.end())
      {
        auto set_two = vecDict[indexTwo].content_.find(dict_it->first);
        if (set_two != vecDict[indexTwo].content_.end())
        {
          std::set< std::string > setTemp;
          auto iBegOne = dict_it->second.begin();
          auto iEndOne = dict_it->second.end();
          auto iBegTwo = set_two->second.begin();
          auto iEndTwo = set_two->second.end();
          std::set_intersection(iBegOne, iEndOne, iBegTwo, iEndTwo, std::inserter(setTemp, setTemp.begin()));
          dictBase.content_[dict_it->first] = setTemp;
        }
        else
        {
          auto temp_it = dict_it->first;
          dict_it++;
          dictBase.content_.erase(temp_it);
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
      informInsufficientData(out);
    }
  }

  void function::print(std::ostream &out, const vecDict &vecDict, std::istream &in)
  {
    while (checkContinueInputWord(in))
    {
      size_t index = findNameDict(out, in, vecDict);
      if (index == vecDict.size())
      {
        continue;
      }
      commandPrintDict(out, vecDict[index]);
    }
  }
}

namespace
{
  bool isEqualName(const stretenskiy::Dictionary &dict, const std::string &nameD)
  {
    return dict.name_ == nameD;
  }

  size_t getIndexDict(std::ostream &out, const std::string &nameD, const stretenskiy::function::vecDict &vecDict)
  {
    using namespace std::placeholders;
    auto iter = std::find_if(vecDict.begin(), vecDict.end(), std::bind(isEqualName, _1, nameD));
    if (iter == vecDict.end())
    {
      stretenskiy::informUnexistDict(out);
    }
    return std::distance(vecDict.begin(), iter);
  }

  size_t findNameDict(std::ostream &out, std::istream &in, const stretenskiy::function::vecDict &vecDict)
  {
    std::string nameD;
    in >> nameD;
    size_t index = getIndexDict(out, nameD, vecDict);
    return index;
  }
}

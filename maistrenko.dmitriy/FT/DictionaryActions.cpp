#include "DictionaryActions.hpp"
#include <ostream>
#include <algorithm>

namespace
{
  using DictElem = std::pair< maistrenko::EngW, maistrenko::RusS >;
  struct Adder
  {
    maistrenko::Dict& toDict;
    void operator()(const DictElem& pair)
    {
      if (toDict.find(pair.first) != toDict.end())
      {
        maistrenko::RusS totalSet = maistrenko::RusS();
        auto toBeg = toDict.at(pair.first).begin();
        auto toEnd = toDict.at(pair.first).end();
        auto pairFBeg = pair.second.begin();
        auto pairFEnd = pair.second.end();

        std::set_union(toBeg, toEnd, pairFBeg, pairFEnd, std::inserter(totalSet, totalSet.end()));
        toDict.at(pair.first) = totalSet;
      }
      else
      {
        toDict.insert(pair);
      }
    }
  };

  struct Intersector
  {
    const maistrenko::Dict& toIntersect;
    maistrenko::Dict& toPutIn;
    void operator()(const DictElem& pair)
    {
      if (toIntersect.find(pair.first) != toIntersect.end())
      {
        maistrenko::EngW key = pair.first;
        maistrenko::RusS value = maistrenko::RusS();
        maistrenko::RusS rusSetInter = toIntersect.at(pair.first);
        maistrenko::RusS newSet = maistrenko::RusS();

        auto pairRusBeg = pair.second.begin();
        auto pairRusEnd = pair.second.end();
        std::set_intersection(pairRusBeg, pairRusEnd, rusSetInter.begin(), rusSetInter.end(), std::inserter(newSet, newSet.end()));
        toPutIn.insert({pair.first, newSet});
      }
    }
  };

  struct Substractor
  {
    const maistrenko::Dict& minuend;
    maistrenko::Dict& toDict;
    void operator()(const DictElem& pair)
    {
      if (minuend.find(pair.first) == minuend.end())
      {
        toDict.insert(pair);
      }
    }
  };
}

void maistrenko::addWord(Dict& dict, const std::pair< EngW, RusS >& dictElem)
{
  if (dict.find(dictElem.first) == dict.end())
  {
    dict.insert(dictElem);
  }
  else
  {
    RusS& rusSet = dict.at(dictElem.first);
    RusS temp = RusS();

    auto first1 = rusSet.begin();
    auto last1 = rusSet.end();
    auto first2 = dictElem.second.begin();
    auto last2 = dictElem.second.end();

    std::set_union(first1, last1, first2, last2, std::inserter(temp, temp.end()));
    rusSet = temp;
  }
}

bool maistrenko::removeWord(Dict& dict, const EngW& engWord)
{
  if (dict.find(engWord) != dict.end())
  {
    dict.erase(engWord);
    return true;
  }
  return false;
}

bool maistrenko::removeTranslate(Dict& dict, const EngW& engWord, const RusW& rusWord)
{
  if (dict.find(engWord) != dict.end())
  {
    RusS& rusSet = dict.at(engWord);
    if (rusSet.find(rusWord) != rusSet.end())
    {
      if (rusSet.size() == 1)
      {
        maistrenko::removeWord(dict, engWord);
      }
      else
      {
        rusSet.erase(rusWord);
      }
      return true;
    }
  }
  return false;
}

maistrenko::Dict maistrenko::unionDicts(const Dict& dict1, const Dict& dict2)
{
  Dict newDict = dict1;
  std::for_each(dict2.begin(), dict2.end(), Adder{newDict});
  return newDict;
}

maistrenko::Dict maistrenko::intersectDicts(const Dict& dict1, const Dict& dict2)
{
  Dict newDict = Dict();
  std::for_each(dict1.begin(), dict1.end(), Intersector{dict2, newDict});
  return newDict;
}

maistrenko::Dict maistrenko::substractDicts(const Dict& dict1, const Dict& dict2)
{
  Dict newDict = Dict();
  std::for_each(dict1.begin(), dict1.end(), Substractor{dict2, newDict});
  return newDict;
}

#include "DictionaryActions.hpp"
#include <ostream>
#include <algorithm>
#include "CommandThrowers.hpp"

namespace
{
  using DictElem = std::pair< maistrenko::EngW, maistrenko::RusS >;

  bool isThereMutual(const maistrenko::RusS& first, const maistrenko::RusS& second)
  {
    maistrenko::RusS intersections = maistrenko::RusS();

    auto first1 = first.begin();
    auto last1 = first.end();
    auto first2 = second.begin();
    auto last2 = second.end();
    std::set_intersection(first1, last1, first2, last2, std::inserter(intersections, intersections.begin()));

    return !intersections.empty();
  }

  struct KeyIntersector
  {
    const maistrenko::Dict& toIntersect;
    bool operator()(const DictElem& pair)
    {
      return toIntersect.find(pair.first) != toIntersect.end();
    }
  };

  struct KeyWithMutualIntersector
  {
    const maistrenko::Dict& toIntersect;
    bool operator()(const DictElem& pair)
    {
      bool total = toIntersect.find(pair.first) != toIntersect.end();
      if (total)
      {
        total = total && isThereMutual(pair.second, toIntersect.at(pair.first));
      }
      return total;
    }
  };

  struct Unioner
  {
    const maistrenko::Dict& toUnion;
    DictElem operator()(const DictElem& pair)
    {
      maistrenko::RusS totalSet = maistrenko::RusS();
      maistrenko::RusS prevSet = toUnion.at(pair.first);

      auto first1 = prevSet.begin();
      auto last1 = prevSet.end();
      auto first2 = pair.second.begin();
      auto last2 = pair.second.end();

      std::set_union(first1, last1, first2, last2, std::inserter(totalSet, totalSet.end()));

      return {pair.first, totalSet};
    }
  };

  struct Intersector
  {
    const maistrenko::Dict& toIntersect;
    DictElem operator()(const DictElem& pair)
    {
      maistrenko::RusS totalSet = maistrenko::RusS();
      maistrenko::RusS prevSet = toIntersect.at(pair.first);

      auto first1 = prevSet.begin();
      auto last1 = prevSet.end();
      auto first2 = pair.second.begin();
      auto last2 = pair.second.end();

      std::set_intersection(first1, last1, first2, last2, std::inserter(totalSet, totalSet.end()));

      return {pair.first, totalSet};
    }
  };

  struct SubstractComp
  {
    bool operator()(const DictElem& pair1, const DictElem& pair2)
    {
      return pair1.first < pair2.first;
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

void maistrenko::removeWord(Dict& dict, const EngW& engWord)
{
  if (dict.find(engWord) != dict.end())
  {
    dict.erase(engWord);
  }
  else
  {
    raiseUnexistingWord();
  }
}

void maistrenko::removeTranslate(Dict& dict, const EngW& engWord, const RusW& rusWord)
{
  bool isRemoved = false;
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
      isRemoved = true;
    }
  }
  if (!isRemoved)
  {
    raiseUnexistingTranslation();
  }
}

maistrenko::Dict maistrenko::unionDicts(const Dict& dict1, const Dict& dict2)
{
  Dict keyInters = Dict();
  std::copy_if(dict2.begin(), dict2.end(), std::inserter(keyInters, keyInters.end()), KeyIntersector{ dict1 });
  Dict newDict = Dict();
  std::transform(keyInters.begin(), keyInters.end(), std::inserter(newDict, newDict.end()), Unioner{ dict1 });
  std::set_union(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(newDict, newDict.end()));

  return newDict;
}

maistrenko::Dict maistrenko::intersectDicts(const Dict& dict1, const Dict& dict2)
{
  Dict keyInters = Dict();
  auto toKeyInters = keyInters.end();
  std::copy_if(dict2.begin(), dict2.end(), std::inserter(keyInters, toKeyInters), KeyWithMutualIntersector{ dict1 });
  Dict newDict = Dict();
  std::transform(keyInters.begin(), keyInters.end(), std::inserter(newDict, newDict.end()), Intersector{ dict1 });

  return newDict;
}

maistrenko::Dict maistrenko::substractDicts(const Dict& dict1, const Dict& dict2)
{
  Dict newDict = Dict();

  auto first1 = dict1.begin();
  auto last1 = dict1.end();
  auto first2 = dict2.begin();
  auto last2 = dict2.end();
  std::set_difference(first1, last1, first2, last2, std::inserter(newDict, newDict.end()), SubstractComp{});

  return newDict;
}

#include "Commands.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "ErrorMessages.h"
#include "IoFmtGuard.h"
#include "IoProcessing.h"

namespace
{
  std::ostream& printTranslation(std::ostream& out, const borisov::TranslationPair& p, size_t sum, char sep);
  std::ostream& printSepTranslation(std::ostream& out, const borisov::TranslationPair& p, size_t sum, char sep, char freqSep);
  std::ostream& printSepWord(std::ostream& out, const std::pair< borisov::Word, borisov::TranslationList >& p, char sep);
  size_t sumCounter(size_t init, const borisov::TranslationPair& p);
  void uniteTL(borisov::TranslationList& first, const borisov::TranslationList& second);
  bool freqComp(const borisov::TranslationPair& first, const borisov::TranslationPair& second);

  template< typename C, typename K >
  bool isLessKeys(const C& c, const K& first, const K& second);
  template< typename C, typename K >
  bool isEqualKeys(const C& c, const K& first, const K& second);

  struct TLComp
  {
    bool operator()(const borisov::TranslationPair& first, const borisov::TranslationPair& second)
    {
      return std::less< borisov::Translation >()(first.first, second.first);
    }
  };
}

void borisov::add(Dict& dict, const Word& word, const Translation& translation, size_t count)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.end())
  {
    TranslationList newTranslationList
    {
      {translation, count}
    };
    dict.insert({word, newTranslationList});
  }
  else
  {
    bool s = wordIter->second.insert({translation, count}).second;
    if (!s)
    {
      throw std::invalid_argument("Translation already exists");
    }
  }
}

void borisov::remove(Dict& dict, const Word& word)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.cend())
  {
    throw std::invalid_argument("No such word");
  }
  dict.erase(wordIter);
}

void borisov::remove(Dict& dict, const Word& word, const Translation& translation)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.end())
  {
    throw std::invalid_argument("No such word");
  }
  auto translationIter = wordIter->second.find(translation);
  if (translationIter == wordIter->second.cend())
  {
    throw std::invalid_argument("No such translation");
  }
  auto checkIsLast = wordIter->second.cbegin();
  if (++checkIsLast == wordIter->second.cend())
  {
    throw std::invalid_argument("Can not delete last translation");
  }
  wordIter->second.erase(translationIter);
}

void borisov::manual(Dict& dict, const Word& word, const Translation& translation, long long count)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.end())
  {
    throw std::invalid_argument("No such word");
  }
  auto translationIter = wordIter->second.find(translation);
  if (translationIter == wordIter->second.end())
  {
    throw std::invalid_argument("No such translation");
  }
  if (static_cast< long long >(translationIter->second) + count > 0)
  {
    translationIter->second += count;
  }
  else
  {
    throw std::invalid_argument("Invalid count");
  }
}

std::ostream& borisov::words(std::ostream& out, const Dict& dict, char sep)
{
  if (dict.empty())
  {
    printMessageEmpty(out);
    return out;
  }
  auto wordIter = dict.cbegin();
  auto wordIterEnd = dict.cend();
  if (wordIter != wordIterEnd)
  {
    out << (wordIter++)->first;
  }
  using namespace std::placeholders;
  std::for_each(
    wordIter, wordIterEnd,
    std::bind(printSepWord, std::ref(out), _1, sep)
  );
  return out;
}

std::ostream& borisov::info(std::ostream& out, const Dict& dict, const Word& word, char freqSep, char sep)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.cend())
  {
    throw std::invalid_argument("No such word");
  }
  auto translationIter = wordIter->second.cbegin();
  auto translationIterEnd = wordIter->second.cend();
  if (translationIter == translationIterEnd)
  {
    printMessageEmpty(out);
    return out;
  }
  using namespace std::placeholders;
  size_t sumCount = std::accumulate(
    translationIter, translationIterEnd,
    0ull, sumCounter
  );
  assert(sumCount != 0);
  printTranslation(out, *(translationIter++), sumCount, freqSep);
  std::for_each(
    translationIter, translationIterEnd,
    std::bind(printSepTranslation, std::ref(out), _1, sumCount, sep, freqSep)
  );
  return out;
}

std::ostream& borisov::translate(std::ostream& out, const Dict& dict, const Word& word)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.cend())
  {
    throw std::invalid_argument("No such word");
  }
  auto maxFreqTranslationIter = std::max_element(
    wordIter->second.cbegin(), wordIter->second.cend(),
    freqComp
  );
  out << maxFreqTranslationIter->first;
  return out;
}

std::ostream& borisov::save(std::ostream& out, const std::string& name, const Dict& dict)
{
  out << name << ' ';
  printDict(out, dict) << '\n';
  return out;
}

void borisov::unite(Dict& result, const Dict& first, const Dict& second)
{
  result = first;
  auto iter = second.cbegin();
  auto iterEnd = second.cend();
  auto firstIterEnd = result.end();
  while (iter != iterEnd)
  {
    auto firstIter = result.find(iter->first);
    if (firstIter == firstIterEnd)
    {
      result.insert({iter->first, iter->second});
    }
    else
    {
      uniteTL(firstIter->second, iter->second);
    }
    ++iter;
  }
}

void borisov::intersect(Dict& result, const Dict& first, const Dict& second)
{
  auto iterFirst = first.cbegin();
  auto iterFirstEnd = first.cend();
  auto iterSecond = second.cbegin();
  auto iterSecondEnd = second.cend();
  while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
  {
    if (isEqualKeys(result, iterFirst->first, iterSecond->first))
    {
      TranslationList tl;
      std::set_intersection(
        iterFirst->second.cbegin(), iterFirst->second.cend(),
        iterSecond->second.cbegin(), iterSecond->second.cend(),
        std::inserter(tl, tl.begin()), TLComp{}
      );
      if (!tl.empty())
      {
        result.insert({iterFirst->first, tl});
      }
      ++iterFirst;
      ++iterSecond;
    }
    else if (isLessKeys(result, iterFirst->first, iterSecond->first))
    {
      ++iterFirst;
    }
    else
    {
      ++iterSecond;
    }
  }
}

void borisov::complement(Dict& result, const Dict& first, const Dict& second)
{
  auto iterFirst = first.cbegin();
  auto iterFirstEnd = first.cend();
  auto iterSecond = second.cbegin();
  auto iterSecondEnd = second.cend();
  while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
  {
    if (isEqualKeys(result, iterFirst->first, iterSecond->first))
    {
      TranslationList tl;
      std::set_difference(
        iterFirst->second.cbegin(), iterFirst->second.cend(),
        iterSecond->second.cbegin(), iterSecond->second.cend(),
        std::inserter(tl, tl.begin()), TLComp{}
      );
      if (!tl.empty())
      {
        result.insert({iterFirst->first, tl});
      }
      ++iterFirst;
      ++iterSecond;
    }
    else if (isLessKeys(result, iterFirst->first, iterSecond->first))
    {
      result.insert({iterFirst->first, iterFirst->second});
      ++iterFirst;
    }
    else
    {
      ++iterSecond;
    }
  }
  while (iterFirst != iterFirstEnd)
  {
    result.insert({iterFirst->first, iterFirst->second});
    ++iterFirst;
  }
}

namespace
{
  std::ostream& printTranslation(std::ostream& out, const borisov::TranslationPair& p, size_t sum, char sep)
  {
    borisov::IoFmtGuard guard(out);
    out << p.first << sep << std::fixed << std::setprecision(2) << (p.second * 1.0) / sum;
    return out;
  }

  std::ostream& printSepTranslation(std::ostream& out, const borisov::TranslationPair& p, size_t sum, char sep, char freqSep)
  {
    out << sep;
    printTranslation(out, p, sum, freqSep);
    return out;
  }

  std::ostream& printSepWord(std::ostream& out, const std::pair< borisov::Word, borisov::TranslationList >& p, char sep)
  {
    out << sep << p.first;
    return out;
  }

  size_t sumCounter(size_t init, const borisov::TranslationPair& p)
  {
    return init + p.second;
  }

  void uniteTL(borisov::TranslationList& first, const borisov::TranslationList& second)
  {
    auto iterFirst = first.begin();
    auto iterFirstEnd = first.end();
    auto iterSecond = second.cbegin();
    auto iterSecondEnd = second.cend();
    while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
    {
      if (isEqualKeys(first, iterFirst->first, iterSecond->first))
      {
        iterFirst->second += iterSecond->second;
        ++iterFirst;
        ++iterSecond;
      }
      else if (isLessKeys(first, iterFirst->first, iterSecond->first))
      {
        ++iterFirst;
      }
      else
      {
        first.insert({iterSecond->first, iterSecond->second});
        ++iterSecond;
      }
    }
    while (iterSecond != iterSecondEnd)
    {
      first.insert({iterSecond->first, iterSecond->second});
      ++iterSecond;
    }
  }

  bool freqComp(const borisov::TranslationPair& first, const borisov::TranslationPair& second)
  {
    return first.second < second.second;
  }

  template< typename C, typename K >
  bool isLessKeys(const C& c, const K& first, const K& second)
  {
    return c.key_comp()(first, second);
  }

  template< typename C, typename K >
  bool isEqualKeys(const C& c, const K& first, const K& second)
  {
    return !isLessKeys(c, first, second) && !isLessKeys(c, second, first);
  }
}

#include "Commands.h"
#include <cassert>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "ErrorMessages.h"
#include "IoFmtGuard.h"

namespace
{
  std::ostream& printTranslation(std::ostream& out, borisov::TranslationList::const_iterator iter, size_t sum, char sep);
  size_t getSumCount(borisov::TranslationList::const_iterator first, borisov::TranslationList::const_iterator last);
  void uniteTL(borisov::TranslationList& first, const borisov::TranslationList& second);
  void intersectTL(borisov::TranslationList& result, const borisov::TranslationList& first, const borisov::TranslationList& second);
  void complementTL(borisov::TranslationList& result, const borisov::TranslationList& first, const borisov::TranslationList& second);
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
  while (wordIter != wordIterEnd)
  {
    out << sep << (wordIter++)->first;
  }
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
  size_t sumCount = getSumCount(translationIter, translationIterEnd);
  printTranslation(out, translationIter++, sumCount, freqSep);
  while (translationIter != translationIterEnd)
  {
    out << sep;
    printTranslation(out, translationIter++, sumCount, freqSep);
  }
  return out;
}

std::ostream& borisov::translate(std::ostream& out, const Dict& dict, const Word& word)
{
  auto wordIter = dict.find(word);
  if (wordIter == dict.cend())
  {
    throw std::invalid_argument("No such word");
  }
  auto translationIter = wordIter->second.cbegin();
  auto translationIterEnd = wordIter->second.cend();
  assert(translationIter != translationIterEnd);
  auto maxFreqTranslationIter = translationIter;
  while (translationIter != translationIterEnd)
  {
    if (translationIter->second > maxFreqTranslationIter->second)
    {
      maxFreqTranslationIter = translationIter;
    }
    ++translationIter;
  }
  out << maxFreqTranslationIter->first;
  return out;
}

std::ostream& borisov::save(std::ostream& out, const std::string& name, const Dict& dict)
{
  out << name;
  auto wordIter = dict.cbegin();
  auto wordIterEnd = dict.cend();
  while (wordIter != wordIterEnd)
  {
    auto translationIter = wordIter->second.cbegin();
    auto translationIterEnd = wordIter->second.cend();
    while (translationIter != translationIterEnd)
    {
      out << ' ' << wordIter->first << ' ' << translationIter->first << ' ' << translationIter->second;
      ++translationIter;
    }
    ++wordIter;
  }
  out << '\n';
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
    if (!result.key_comp()(iterFirst->first, iterSecond->first) && !result.key_comp()(iterSecond->first, iterFirst->first))
    {
      TranslationList tl;
      intersectTL(tl, iterFirst->second, iterSecond->second);
      if (!tl.empty())
      {
        result.insert({iterFirst->first, tl});
      }
      ++iterFirst;
      ++iterSecond;
    }
    else if (result.key_comp()(iterFirst->first, iterSecond->first))
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
    if (!result.key_comp()(iterFirst->first, iterSecond->first) && !result.key_comp()(iterSecond->first, iterFirst->first))
    {
      TranslationList tl;
      complementTL(tl, iterFirst->second, iterSecond->second);
      if (!tl.empty())
      {
        result.insert({iterFirst->first, tl});
      }
      ++iterFirst;
      ++iterSecond;
    }
    else if (result.key_comp()(iterFirst->first, iterSecond->first))
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
  std::ostream& printTranslation(std::ostream& out, borisov::TranslationList::const_iterator iter, size_t sum, char sep)
  {
    assert(sum != 0);
    borisov::IoFmtGuard guard(out);
    out << iter->first << sep << std::fixed << std::setprecision(2) << (iter->second * 1.0) / sum;
    return out;
  }

  size_t getSumCount(borisov::TranslationList::const_iterator first, borisov::TranslationList::const_iterator last)
  {
    size_t result = 0;
    while (first != last)
    {
      result += (first++)->second;
    }
    return result;
  }

  void uniteTL(borisov::TranslationList& first, const borisov::TranslationList& second)
  {
    auto iterFirst = first.begin();
    auto iterFirstEnd = first.end();
    auto iterSecond = second.cbegin();
    auto iterSecondEnd = second.cend();
    while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
    {
      if (!first.key_comp()(iterFirst->first, iterSecond->first) && !first.key_comp()(iterSecond->first, iterFirst->first))
      {
        iterFirst->second += iterSecond->second;
        ++iterFirst;
        ++iterSecond;
      }
      else if (first.key_comp()(iterFirst->first, iterSecond->first))
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

  void intersectTL(borisov::TranslationList& result, const borisov::TranslationList& first, const borisov::TranslationList& second)
  {
    auto iterFirst = first.cbegin();
    auto iterFirstEnd = first.cend();
    auto iterSecond = second.cbegin();
    auto iterSecondEnd = second.cend();
    while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
    {
      if (!result.key_comp()(iterFirst->first, iterSecond->first) && !result.key_comp()(iterSecond->first, iterFirst->first))
      {
        result.insert({iterFirst->first, iterFirst->second});
        ++iterFirst;
        ++iterSecond;
      }
      else if (result.key_comp()(iterFirst->first, iterSecond->first))
      {
        ++iterFirst;
      }
      else
      {
        ++iterSecond;
      }
    }
  }

  void complementTL(borisov::TranslationList& result, const borisov::TranslationList& first, const borisov::TranslationList& second)
  {
    auto iterFirst = first.cbegin();
    auto iterFirstEnd = first.cend();
    auto iterSecond = second.cbegin();
    auto iterSecondEnd = second.cend();
    while (iterFirst != iterFirstEnd && iterSecond != iterSecondEnd)
    {
      if (!result.key_comp()(iterFirst->first, iterSecond->first) && !result.key_comp()(iterSecond->first, iterFirst->first))
      {
        ++iterFirst;
        ++iterSecond;
      }
      else if (result.key_comp()(iterFirst->first, iterSecond->first))
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
}

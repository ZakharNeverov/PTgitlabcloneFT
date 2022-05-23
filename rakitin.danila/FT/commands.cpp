#include "commands.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

#include "dictionary_utils.h"
#include "io-utils.h"

void rakitin::add(dictionaries& data, const std::string& dataname, const std::string& path)
{
  data.insert({ dataname, rakitin::getDictionary(path) });
}

rakitin::ref_finder rakitin::findWord(const dictionaries& data, const std::string& dataname, const std::string& word)
{
  dictionary dict = getDictionaryFromData(data, dataname);
  using namespace std::placeholders;
  auto func = std::bind(rakitin::isRefsForWord, word, _1);
  rakitin::dictionary::iterator res = std::find_if(dict.begin(), dict.end(), func);
  bool found = res != dict.end();
  if (found) {
    return std::make_pair(*res, found);
  }
  return std::make_pair(getEmptyRef(), found);
}

void rakitin::printAll(const dictionaries& data, const std::string& dataname, std::ostream& stream)
{
  dictionary dict = getDictionaryFromData(data, dataname);
  auto begin = dict.begin();
  auto end = dict.end();
  while (begin != end) {
    rakitin::printWord(*begin, stream);
    begin++;
    if (begin != end) {
      stream << '\n';
    }
  }
}

void rakitin::merge(dictionaries& data, const std::string& dataname1, const std::string& dataname2)
{
  dictionary dict1 = getDictionaryFromData(data, dataname1);
  dictionary dict2 = getDictionaryFromData(data, dataname2);
  dictionary result;

  dictionary::iterator it1 = dict1.begin();
  dictionary::iterator it2 = dict2.begin();

  while (it1 != dict1.end() || it2 != dict2.end()) {
    if (it1 != dict1.end()) {
      for (std::set< int >::iterator itt1 = it1->second.begin(); itt1 != it1->second.end(); itt1++) {
        addWord(result, it1->first, *itt1);
      }
      it1++;
    }
    if (it2 != dict2.end()) {
      for (std::set< int >::iterator itt2 = it2->second.begin(); itt2 != it2->second.end(); itt2++) {
        addWord(result, it2->first, *itt2);
      }
      it2++;
    }
  }

  data.erase(dataname2);
  data.erase(dataname1);
  data.insert({ dataname1, result });
}

void rakitin::intersection(dictionaries& data, const std::string& dataname1, const std::string& dataname2)
{
  dictionary dict1 = getDictionaryFromData(data, dataname1);
  dictionary dict2 = getDictionaryFromData(data, dataname2);
  dictionary result;

  dictionary::iterator it1 = dict1.begin();
  dictionary::iterator it2 = dict2.begin();

  while (it1 != dict1.end() && it2 != dict2.end()) {
    if (it1 != dict1.end()) {
      while (it2 != dict2.end() && it2->first < it1->first) {
        it2++;
      }
      if (it2 != dict2.end() && it2->first == it1->first) {
        std::set< int >::iterator itt1 = it1->second.begin();
        std::set< int >::iterator itt2 = it2->second.begin();
        while (itt1 != it1->second.end() || itt2 != it2->second.end()) {
          if (itt1 != it1->second.end()) {
            addWord(result, it1->first, *itt1);
            itt1++;
          }
          if (itt2 != it2->second.end()) {
            addWord(result, it2->first, *itt2);
            itt2++;
          }
        }
      }
      it1++;
    }
  }

  data.insert({ dataname1 + "&" + dataname2 + "_intersection", result });
}

double rakitin::similarity(const dictionaries& data, const std::string& dataname1, const std::string& dataname2)
{
  int all = 0;
  int similar = 0;

  dictionary dict1 = getDictionaryFromData(data, dataname1);
  dictionary dict2 = getDictionaryFromData(data, dataname2);

  dictionary::iterator it1 = dict1.begin();
  dictionary::iterator it2 = dict2.begin();

  while (it1 != dict1.end() && it2 != dict2.end()) {
    if (it1 != dict1.end()) {
      while (it2 != dict2.end() && it2->first < it1->first) {
        all += it2->second.size();
        it2++;
      }
      if (it2 != dict2.end() && it2->first == it1->first) {
        similar += std::min(it2->second.size(), it1->second.size()) * 2;
      }
    }
    all += it1->second.size();
    it1++;
  }
  while (it1 != dict1.end() || it2 != dict2.end()) {
    if (it1 != dict1.end()) {
      all += it1->second.size();
      it1++;
    }
    if (it2 != dict2.end()) {
      all += it2->second.size();
      it2++;
    }
  }
  return static_cast< double >(similar) / static_cast< double >(all);
}

void rakitin::deleteDictionary(dictionaries& data, const std::string& dataname1)
{
  data.erase(dataname1);
}

void rakitin::rename(dictionaries& data, const std::string& dataname1, const std::string& newName)
{
  auto dictionaryPtr = data.find(dataname1);
  if (dictionaryPtr == data.end()) {
    throw std::logic_error("cant find dictionary " + dataname1);
  }
  dictionary newDictionary = dictionaryPtr->second;
  data.erase(dictionaryPtr);
  data.emplace(newName, newDictionary);
}

void rakitin::printAllDatanames(const dictionaries& data, std::ostream& stream)
{
  for (dictionaries::const_iterator iter = data.cbegin(); iter != data.cend(); ) {
    auto begin = iter->second.begin();
    auto end = iter->second.end();
    stream << "Dictionary: " << iter->first << "\n";
    while (begin != end) {
      rakitin::printWord(*begin, stream);
      begin++;
      if (begin != end) {
        stream << '\n';
      }
    }
    iter++;
    if (iter != data.cend()) {
      stream << '\n';
    }
  }
}

void rakitin::clear(dictionaries& data, const std::string& dataname)
{
  auto dictionaryPtr = data.find(dataname);
  if (dictionaryPtr == data.end()) {
    throw std::logic_error("cant find dictionary " + dataname);
  }
  dictionary empty;
  data.erase(dictionaryPtr);
  data.insert({ dataname, empty });
}

rakitin::dictionary rakitin::getDictionaryFromData(const dictionaries& data, const std::string& dataname)
{
  auto dictionaryPtr = data.find(dataname);
  if (dictionaryPtr == data.end()) {
    throw std::logic_error("cant find dictionary " + dataname);
  }
  return dictionaryPtr->second;
}

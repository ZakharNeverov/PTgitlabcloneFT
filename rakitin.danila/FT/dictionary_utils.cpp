#include "dictionary_utils.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

void rakitin::addWord(dictionary& dictionary, const std::string& word, int line)
{
  using namespace std::placeholders;
  auto func = std::bind(isRefsForWord, word, _1);
  auto refs = std::find_if(dictionary.begin(), dictionary.end(), func);
  word_refs ref;
  if (refs != dictionary.end()) {
    refs->second.insert(line);
  }
  else {
    ref = word_refs(word, std::set< int >());
    ref.second.insert(line);
    dictionary.push_back(ref);
  }
}

bool rakitin::isRefsForWord(const std::string& word, const word_refs& refs)
{
  return refs.first == word;
}

void rakitin::printWord(const word_refs& ref, std::ostream& stream)
{
  stream << ref.first << " ";
  std::set< int > refs = ref.second;
  auto begin = refs.begin();
  auto end = refs.end();
  while (begin != end) {
    int n = *begin;
    if (++begin != end) {
      stream << n << ", ";
    }
    else {
      stream << n;
    }
  }
}

rakitin::word_refs rakitin::getEmptyRef() {
  return word_refs();
}

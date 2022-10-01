#ifndef DICTIONARY_UTILS_HPP
#define DICTIONARY_UTILS_HPP

#include "dictionary_usings.h"

namespace rakitin {
  void addWord(dictionary& dictionary, const std::string& word, int line);
  bool isRefsForWord(const std::string& word, const word_refs& refs);
  void printWord(const word_refs& ref, std::ostream& stream);
  word_refs getEmptyRef();
}

#endif

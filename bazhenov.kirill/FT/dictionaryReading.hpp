#ifndef DICTIONARYREADING_HPP
#define DICTIONARYREADING_HPP

#include <map>
#include <fstream>

namespace bazhenov {
  using freq = std::map< std::string, int >;

  std::string createName(const std::string& str);
  freq readDictionary(std::istream& file);
  void readWord(std::string& word, freq& dictionary);
}

#endif
